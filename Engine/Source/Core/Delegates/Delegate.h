#pragma once

#include "DelegateBase.h"
#include "Memory/Allocator.h"

#ifndef JE_DELEGATE_ALLOC_SIZE
	/**
	 * Inline allocator stack-buffer size.\n
	 * 32 bytes should be enough for most cases, but leave 64 to prevent heap allocations for lambdas with captures.
	 */
	#define JE_DELEGATE_ALLOC_SIZE 64
#endif

namespace JE
{
	template <class TReturn, class... TArgs>
	class JE_API FDelegateInstance final
	{
		//- Types ------------------------
	private:
		using TDelegateBase = IDelegateBase<TReturn, TArgs...>;

		//- Variables --------------------
	private:
		/** Inlined allocator for storing delegate instances. */
		FInlineAllocator<JE_DELEGATE_ALLOC_SIZE> Allocator;

		/** Delegate's instance handle. */
		FDelegateHandle Handle;

		//- Lifecycle --------------------
	public:
		FDelegateInstance()
			: Allocator()
			, Handle()
		{
		}

		~FDelegateInstance() noexcept
		{
			Deinit<TDelegateBase>();
		}

		FDelegateInstance(const FDelegateInstance& _delegate) noexcept
		{
			if (*this == _delegate)
			{
				return;
			}

			Allocator = _delegate.Allocator;
			Handle = _delegate.Handle;
		}

		FDelegateInstance& operator=(const FDelegateInstance& _delegate) noexcept
		{
			if (*this == _delegate)
			{
				return *this;
			}

			Allocator = _delegate.Allocator;
			Handle = _delegate.Handle;
			return *this;
		}

		FDelegateInstance(FDelegateInstance&& _delegate) noexcept
		{
			if (*this == _delegate)
			{
				return;
			}

			Allocator = std::move(_delegate.Allocator);
			Handle = std::move(_delegate.Handle);
		}

		FDelegateInstance& operator=(FDelegateInstance&& _delegate) noexcept
		{
			if (*this == _delegate)
			{
				return *this;
			}

			Allocator = std::move(_delegate.Allocator);
			Handle = std::move(_delegate.Handle);
			return *this;
		}

		bool operator==(const FDelegateInstance& _delegate) const noexcept
		{
			// TODO: theoretically, shouldn't exist 2 delegates with equal handles, should it?
			return Handle == _delegate.Handle && Allocator == _delegate.Allocator;
		}

		//- Methods ----------------------
	public:
		template <class TDelegate, class... TArgs2>
			requires std::is_base_of_v<TDelegateBase, TDelegate>
		static FDelegateInstance<TReturn, TArgs...> Create(TArgs2&&... _args)
		{
			FDelegateInstance<TReturn, TArgs...> delegateInstance;
			delegateInstance.template Init<TDelegate>(std::forward<TArgs2>(_args)...);
			JE_ASSERT(delegateInstance.IsBound());

			return delegateInstance;
		}

		template <class TDelegate = TDelegateBase>
			requires std::is_base_of_v<TDelegateBase, TDelegate>
		TDelegate* Get()
		{
			return Allocator.Cast<TDelegate>();
		}

		template <class TDelegate = TDelegateBase>
			requires std::is_base_of_v<TDelegateBase, TDelegate>
		const TDelegate* Get() const
		{
			return Allocator.Cast<TDelegate>();
		}

		FDelegateHandle GetHandle() const
		{
			return Handle;
		}

		template <class TDelegate, class... TArgs2>
			requires std::is_base_of_v<TDelegateBase, TDelegate>
		void Init(TArgs2&&... _args)
		{
			Allocator.Allocate(sizeof(TDelegate));
			TDelegate* const delegateBase = Allocator.Cast<TDelegate>();

			JE_ASSERT(delegateBase);
			std::construct_at(delegateBase, std::forward<TArgs2>(_args)...);

			Handle = FDelegateHandle(FDelegateHandle::Generate);
		}

		template <class TDelegate = TDelegateBase>
			requires std::is_base_of_v<TDelegateBase, TDelegate>
		void Deinit()
		{
			if (Allocator.IsEmpty())
			{
				return;
			}

			TDelegate* const delegateBase = Allocator.Cast<TDelegate>();
			JE_ASSERT(delegateBase);

			std::destroy_at<TDelegate>(delegateBase);
			Allocator.Free();
			Handle.Reset();
		}

		TReturn Execute(TArgs&&... _args)
		{
			TDelegateBase* delegateBase = Get();
			JE_ASSERT(delegateBase && IsBound());

			return delegateBase->Execute(std::forward<TArgs>(_args)...);
		}

		TReturn ExecuteIfBound(TArgs&&... _args)
		{
			if (IsBound())
			{
				TDelegateBase* delegateBase = Get();
				JE_ASSERT(delegateBase);

				return delegateBase->Execute(std::forward<TArgs>(_args)...);
			}

			return TReturn{};
		}

		inline bool IsBound() const noexcept
		{
			return !Allocator.IsEmpty() && Handle.IsValid();
		}

		inline const void* GetOwner() const noexcept
		{
			const TDelegateBase* delegateBase = Get();
			return delegateBase ? delegateBase->GetOwner() : nullptr;
		}
	};

	// TODO: Test delegates behavior!
	/**
	 * Single-binding delegate implementation.
	 */
	template <class TReturn, class... TArgs>
	class JE_API FDelegate
	{
		//- Types ------------------------
	private:
		using TDelegateBase = IDelegateBase<TReturn, TArgs...>;

		using TStaticDelegateBase = FStaticDelegateBase<TReturn, TArgs...>;

		template <class TObject>
		using TRawDelegateBase = FRawDelegateBase<false, TObject, TReturn, TArgs...>;

		template <class TObject>
		using TConstRawDelegateBase = FRawDelegateBase<true, TObject, TReturn, TArgs...>;

		template <class TLambda>
		using TLambdaDelegateBase = FLambdaDelegateBase<TLambda, TReturn, TArgs...>;

		template <class TObject>
		using TSharedRawDelegateBase = FSharedRawDelegateBase<false, TObject, TReturn, TArgs...>;

		template <class TObject>
		using TConstSharedRawDelegateBase = FSharedRawDelegateBase<true, TObject, TReturn, TArgs...>;

		///

		using TStaticFunction = TReturn (*)(TArgs...);

		template <class TObject>
		using TMemberFunction = typename TMemberFunctionPtr<false, TObject, TReturn, TArgs...>::Type;

		template <class TObject>
		using TConstMemberFunction = typename TMemberFunctionPtr<true, TObject, TReturn, TArgs...>::Type;

		//- Variables --------------------
	private:
		FDelegateInstance<TReturn, TArgs...> Instance;

		/** Delegates handler. Expects to be unique for each new one. */
		FDelegateHandle Handle;

		//- Lifecycle --------------------
	public:
		FDelegate()
			: Instance(FDelegateInstance<TReturn, TArgs...>())
		{
		}

		~FDelegate()
		{
			Clear();
		}

		FDelegate(const FDelegate& _delegate) noexcept
		{
			Instance = _delegate.Instance;
		}

		FDelegate& operator=(const FDelegate& _delegate) noexcept
		{
			if (*this == _delegate)
			{
				return *this;
			}

			Instance = _delegate.Instance;
			return *this;
		}

		FDelegate(FDelegate&& _delegate) noexcept
		{
			Instance = std::move(_delegate.Instance);
		}

		FDelegate& operator=(FDelegate&& _delegate) noexcept
		{
			if (*this == _delegate)
			{
				return *this;
			}

			Instance = std::move(_delegate.Instance);
			return *this;
		}

		bool operator==(const FDelegate& _delegate) noexcept
		{
			// TODO: theoretically, shouldn't exist 2 delegates with equal handles, should it?
			return Instance == _delegate.Instance;
		}

		//- Methods ----------------------
	public:
		FDelegateHandle GetHandle() const noexcept
		{
			return Instance.GetHandle();
		}

		TReturn Execute(TArgs&&... _args)
		{
			return Instance.Execute(std::forward<TArgs>(_args)...);
		}

		void Clear()
		{
			Instance.template Deinit<TDelegateBase>();
		}

		inline bool IsBound() const
		{
			return Instance.IsBound();
		}

		inline bool IsBoundTo(void* _object) const
		{
			return Instance.GetOwner() == _object;
		}

		void Bind(TStaticFunction _staticFunction)
		{
			Clear();
			Instance.template Init<TStaticDelegateBase>(_staticFunction);
		}

		template <class TObject>
		void Bind(TObject* _object, TMemberFunction<TObject> _method)
		{
			static_assert(!std::is_const_v<TObject>,
				"Attempting to bind a delegate with a const object pointer and non-const member function.");

			Clear();
			Instance.template Init<TRawDelegateBase<TObject>>(_object, _method);
		}

		template <class TObject>
		void Bind(const TObject* _object, TConstMemberFunction<TObject> _method)
		{
			Clear();
			Instance.template Init<TConstRawDelegateBase<const TObject>>(_object, _method);
		}

		template <class TLambda>
		void Bind(TLambda&& _lambda)
		{
			Clear();

			using TLambdaType = std::decay_t<TLambda>;
			Instance.template Init<TLambdaDelegateBase<TLambdaType>>(std::forward<TLambdaType>(_lambda));
		}

		template <class TObject>
		void Bind(const std::shared_ptr<TObject>& _object, TMemberFunction<TObject> _method)
		{
			static_assert(!std::is_const_v<TObject>,
				"Attempting to bind a delegate with a const object pointer and non-const member function.");

			Clear();
			Instance.template Init<TSharedRawDelegateBase<TObject>>(_object, _method);
		}

		template <class TObject>
		void Bind(const std::shared_ptr<const TObject>& _object, TConstMemberFunction<TObject> _method)
		{
			Clear();
			Instance.template Init<TConstSharedRawDelegateBase<const TObject>>(_object, _method);
		}

		static FDelegate CreateStaticDelegate(TStaticFunction _staticFunctionPtr)
		{
			FDelegate delegate;
			delegate.Bind(_staticFunctionPtr);
			return delegate;
		}

		template <class TObject>
		static FDelegate CreateRawDelegate(TObject* _object, TMemberFunction<TObject> _method)
		{
			FDelegate delegate;
			delegate.Bind(_object, _method);
			return delegate;
		}

		template <class TObject>
		static FDelegate CreateRawDelegate(const TObject* _object, TConstMemberFunction<TObject> _method)
		{
			FDelegate delegate;
			delegate.Bind(_object, _method);
			return delegate;
		}

		template <class TLambda>
		static FDelegate CreateLambdaDelegate(TLambda&& _lambda)
		{
			FDelegate delegate;
			delegate.Bind(std::forward<TLambda>(_lambda));
			return delegate;
		}

		template <class TObject>
		static FDelegate CreateSharedRawDelegate(const std::shared_ptr<TObject>& _object, TMemberFunction<TObject> _method)
		{
			FDelegate delegate;
			delegate.Bind(_object, _method);
			return delegate;
		}

		template <class TObject>
		static FDelegate CreateSharedRawDelegate(const std::shared_ptr<const TObject>& _object, TConstMemberFunction<TObject> _method)
		{
			FDelegate delegate;
			delegate.Bind(_object, _method);
			return delegate;
		}
	};

} // namespace JE
