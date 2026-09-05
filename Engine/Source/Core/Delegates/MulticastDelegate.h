#pragma once

#include "Assertions/Check.h"
#include "Delegates/Delegate.h"

namespace JE
{
	/**
	 * Implementation of a delegate with multiple bindings.\n
	 * The invocation order is not guaranteed.
	 */
	template <class... TArgs>
	class JE_API FMulticastDelegate
	{
		//- Types ------------------------
	private:
		using TDelegateInstance = FDelegateInstance<void, TArgs...>;

		using TStaticDelegateBase = FStaticDelegateBase<void, TArgs...>;

		template <class TObject>
		using TRawDelegateBase = FRawDelegateBase<false, TObject, void, TArgs...>;

		template <class TObject>
		using TConstRawDelegateBase = FRawDelegateBase<true, TObject, void, TArgs...>;

		template <class TLambda>
		using TLambdaDelegateBase = FLambdaDelegateBase<TLambda, void, TArgs...>;

		template <class TObject>
		using TSharedRawDelegateBase = FSharedRawDelegateBase<false, TObject, void, TArgs...>;

		template <class TObject>
		using TConstSharedRawDelegateBase = FSharedRawDelegateBase<true, TObject, void, TArgs...>;

		///

		using TStaticFunction = void (*)(TArgs...);

		template <class TObject>
		using TMemberFunction = typename TMemberFunctionPtr<false, TObject, void, TArgs...>::Type;

		template <class TObject>
		using TConstMemberFunction = typename TMemberFunctionPtr<true, TObject, void, TArgs...>::Type;

		//- Variables --------------------
	private:
		std::vector<TDelegateInstance> InvocationList;

		//- Lifecycle --------------------
	public:
		FMulticastDelegate()
			: InvocationList()
		{
			InvocationList.reserve(4);
		}

		~FMulticastDelegate()
		{
			Clear();
		}

		FMulticastDelegate(const FMulticastDelegate& _delegate) noexcept
		{
			InvocationList = _delegate.InvocationList;
		}

		FMulticastDelegate& operator=(const FMulticastDelegate& _delegate) noexcept
		{
			if (*this == _delegate)
			{
				return *this;
			}

			InvocationList = _delegate.InvocationList;
			return *this;
		}

		FMulticastDelegate(FMulticastDelegate&& _delegate) noexcept
		{
			InvocationList = std::move(_delegate.InvocationList);
		}

		FMulticastDelegate& operator=(FMulticastDelegate&& _delegate) noexcept
		{
			if (*this == _delegate)
			{
				return *this;
			}

			InvocationList = std::move(_delegate.InvocationList);

			return *this;
		}

		bool operator==(const FMulticastDelegate& _delegate) const noexcept
		{
			return InvocationList == _delegate.InvocationList;
		}

		//- Methods ----------------------
	public:
		void Broadcast(TArgs&&... _args)
		{
			// TODO: Lock()?
			for (TDelegateInstance& delegateInstance : InvocationList)
			{
				JE_CHECK_CONTINUE_F(delegateInstance.IsBound(), "Delegate instance is not bound during broadcasting process.");
				delegateInstance.Execute(std::forward<TArgs>(_args)...);
			}
		}

		void Clear()
		{
			InvocationList.clear();
		}

		inline bool IsBound() const
		{
			return !InvocationList.empty();
		}

		inline bool IsBoundTo(void* _object) const noexcept
		{
			for (const TDelegateInstance& delegateInstance : InvocationList)
			{
				if (delegateInstance.GetOwner() == _object)
				{
					return true;
				}
			}

			return false;
		}

		inline bool Contains(const FDelegateHandle& _delegateHandle) const noexcept
		{
			for (TDelegateInstance& delegateInstance : InvocationList)
			{
				if (delegateInstance.GetHandle() == _delegateHandle)
				{
					return true;
				}
			}

			return false;
		}

		FDelegateHandle Add(TStaticFunction _staticFunctionPtr)
		{
			return Add(TDelegateInstance::template Create<TStaticDelegateBase>(_staticFunctionPtr));
		}

		template <class TObject>
		FDelegateHandle Add(TObject* _object, TMemberFunction<TObject> _method)
		{
			static_assert(!std::is_const_v<TObject>,
				"Attempting to bind a delegate with a const object pointer and non-const member function.");

			return Add(TDelegateInstance::template Create<TRawDelegateBase<TObject>>(_object, _method));
		}

		template <class TObject>
		FDelegateHandle Add(TObject* _object, TConstMemberFunction<TObject> _method)
		{
			return Add(TDelegateInstance::template Create<TConstRawDelegateBase<const TObject>>(_object, _method));
		}

		template <class TLambda>
		FDelegateHandle Add(TLambda&& _lambda)
		{
			return Add(TDelegateInstance::template Create<TLambdaDelegateBase<TLambda>>(std::forward<TLambda>(_lambda)));
		}

		template <class TObject>
		FDelegateHandle Add(const std::shared_ptr<TObject>& _object, TMemberFunction<TObject> _method)
		{
			static_assert(!std::is_const_v<TObject>,
				"Attempting to bind a delegate with a const object pointer and non-const member function.");

			return Add(TDelegateInstance::template Create<TSharedRawDelegateBase<TObject>>(_object, _method));
		}

		template <class TObject>
		FDelegateHandle Add(const std::shared_ptr<TObject>& _object, TConstMemberFunction<TObject> _method)
		{
			return Add(TDelegateInstance::template Create<TConstSharedRawDelegateBase<const TObject>>(_object, _method));
		}

		bool Remove(FDelegateHandle& _delegateHandle)
		{
			if (_delegateHandle.IsValid())
			{
				for (size_t Idx = 0; Idx < InvocationList.size(); Idx++)
				{
					TDelegateInstance& delegate = InvocationList[Idx];
					if (delegate.GetHandle() == _delegateHandle)
					{
						std::swap(InvocationList[Idx], InvocationList.back());
						InvocationList.pop_back();

						_delegateHandle.Reset();
						return true;
					}
				}
			}

			return false;
		}

		bool RemoveAll(void* _object)
		{
			bool bRemoved = false;
			if (_object)
			{
				for (int64 Idx = InvocationList.size() - 1; Idx >= 0; --Idx)
				{
					const TDelegateInstance& delegateInstance = InvocationList[Idx];
					if (delegateInstance.GetOwner() == _object)
					{
						std::swap(InvocationList[Idx], InvocationList.back());
						InvocationList.pop_back();
						bRemoved = true;
					}
				}
			}

			return bRemoved;
		}

	protected:
		FDelegateHandle Add(TDelegateInstance&& _delegateInstance)
		{
			for (size_t Idx = 0; Idx < InvocationList.size(); Idx++)
			{
				if (!InvocationList[Idx].IsBound())
				{
					InvocationList[Idx].Deinit();
					InvocationList[Idx] = std::move(_delegateInstance);
					return InvocationList[Idx].GetHandle();
				}
			}

			InvocationList.emplace_back(std::move(_delegateInstance));
			return InvocationList.back().GetHandle();
		}
	};

} // namespace JE
