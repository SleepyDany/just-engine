#pragma once

#include "DelegateBase.h"
#include "Memory/Allocator.h"

#ifndef JE_DELEGATE_ALLOC_SIZE
	/** Size for the inlined allocator. */
	#define JE_DELEGATE_ALLOC_SIZE 64
#endif

namespace JE
{
	// TODO: Test delegates behavior!
	/**
	 * Single-binding delegate implementation.
	 */
	template <class TReturn, class... TArgs>
	class FDelegate
	{
		//- Types ------------------------
	private:
		using TDelegateInstance = IDelegateBase<TReturn, TArgs...>;

		using TStaticDelegateInstance = FStaticDelegateInstance<TReturn, TArgs...>;

		template <class TObject>
		using TRawDelegateInstance = FRawDelegateInstance<false, TObject, TReturn, TArgs...>;

		template <class TObject>
		using TConstRawDelegateInstance = FRawDelegateInstance<true, TObject, TReturn, TArgs...>;

		template <class TLambda>
		using TLambdaDelegateInstance = FLambdaDelegateInstance<TLambda, TReturn, TArgs...>;

		template <class TObject>
		using TSharedRawDelegateInstance = FSharedRawDelegateInstance<false, TObject, TReturn, TArgs...>;

		template <class TObject>
		using TConstSharedRawDelegateInstance = FSharedRawDelegateInstance<true, TObject, TReturn, TArgs...>;

		///

		using TStaticFunction = TReturn (*)(TArgs...);

		template <class TObject>
		using TMemberFunction = typename TMemberFunctionPtr<false, TObject, TReturn, TArgs...>::Type;

		template <class TObject>
		using TConstMemberFunction = typename TMemberFunctionPtr<true, TObject, TReturn, TArgs...>::Type;

		//- Variables --------------------
	private:
		/** Inlined allocator for storing delegate instances. */
		FInlineAllocator<JE_DELEGATE_ALLOC_SIZE> Allocator;

		/** Delegates handler. Expects to be unique for each new one. */
		FDelegateHandle Handle;

		//- Lifecycle --------------------
	public:
		FDelegate()
			: Allocator()
			, Handle(FDelegateHandle::Generate)
		{
		}

		~FDelegate()
		{
			Handle.Reset();
			Clear();
		}

		FDelegate(const FDelegate& _delegate) noexcept
		{
			Allocator = _delegate.Allocator;
			Handle = _delegate.Handle;
		}

		FDelegate& operator=(const FDelegate& _delegate) noexcept
		{
			if (*this == _delegate)
			{
				return *this;
			}

			Allocator = _delegate.Allocator;
			Handle = _delegate.Handle;

			return *this;
		}

		FDelegate(FDelegate&& _delegate) noexcept
		{
			Allocator = std::move(_delegate.Allocator);
			Handle = _delegate.Handle;
			_delegate.Handle.Reset();
		}

		FDelegate& operator=(FDelegate&& _delegate) noexcept
		{
			if (*this == _delegate)
			{
				return *this;
			}

			Allocator = std::move(_delegate.Allocator);
			Handle = _delegate.Handle;
			_delegate.Handle.Reset();

			return *this;
		}

		bool operator==(const FDelegate& _delegate) noexcept
		{
			// TODO: theoretically, shouldn't exist 2 delegates with equal handles, should it?
			return Handle == _delegate.Handle && Allocator == _delegate.Allocator;
		}

		//- Methods ----------------------
	public:
		/** Get delegate handle. */
		FDelegateHandle GetHandle() const
		{
			return Handle;
		}

		TReturn Execute(TArgs&&... _args)
		{
			TDelegateInstance* delegateInstance = Allocator.Cast<TDelegateInstance>();
			JE_ASSERT(delegateInstance);
			return delegateInstance->Execute(std::forward<TArgs>(_args)...);
		}

		void Clear()
		{
			if (IsBound())
			{
				TDelegateInstance* const delegateInstance = static_cast<TDelegateInstance* const>(Allocator.GetData());
				JE_ASSERT(delegateInstance);

				std::destroy_at<TDelegateInstance>(delegateInstance);
				Allocator.Free();
			}
		}

		inline bool IsBound() const
		{
			return !Allocator.IsEmpty();
		}

		void BindStatic(TStaticFunction _staticFunctionPtr)
		{
			Clear();

			Allocator.Allocate(sizeof(TStaticDelegateInstance));
			TStaticDelegateInstance* delegateInstance = Allocator.Cast<TStaticDelegateInstance>();

			JE_ASSERT(delegateInstance);
			std::construct_at(delegateInstance, _staticFunctionPtr);
		}

		template <class TObject>
		void BindRaw(TObject* _object, TMemberFunction<TObject> _methodPtr)
		{
			static_assert(!std::is_const_v<TObject>,
				"Attempting to bind a delegate with a const object pointer and non-const member function.");

			Clear();

			Allocator.Allocate(sizeof(TRawDelegateInstance<TObject>));
			TRawDelegateInstance<TObject>* delegateInstance = Allocator.Cast<TRawDelegateInstance<TObject>>();

			JE_ASSERT(delegateInstance);
			std::construct_at(delegateInstance, _object, _methodPtr);
		}

		template <class TObject>
		void BindRaw(const TObject* _object, TConstMemberFunction<TObject> _methodPtr)
		{
			Clear();

			Allocator.Allocate(sizeof(TConstRawDelegateInstance<const TObject>));
			TConstRawDelegateInstance<const TObject>* delegateInstance = Allocator.Cast<TConstRawDelegateInstance<const TObject>>();

			JE_ASSERT(delegateInstance);
			std::construct_at(delegateInstance, _object, _methodPtr);
		}

		template <class TLambda>
		void BindLambda(TLambda&& _lambda)
		{
			Clear();

			using TLambdaType = std::decay_t<TLambda>;
			Allocator.Allocate(sizeof(TLambdaDelegateInstance<TLambdaType>));
			TLambdaDelegateInstance<TLambdaType>* delegateInstance = Allocator.Cast<TLambdaDelegateInstance<TLambdaType>>();

			JE_ASSERT(delegateInstance);
			std::construct_at(delegateInstance, std::forward<TLambdaType>(_lambda));
		}

		template <class TObject>
		void BindSharedRaw(const std::shared_ptr<TObject>& _object, TMemberFunction<TObject> _method)
		{
			static_assert(!std::is_const_v<TObject>,
				"Attempting to bind a delegate with a const object pointer and non-const member function.");

			Clear();

			Allocator.Allocate(sizeof(TSharedRawDelegateInstance<TObject>));
			TSharedRawDelegateInstance<TObject>* delegateInstance = Allocator.Cast<TSharedRawDelegateInstance<TObject>>();

			JE_ASSERT(delegateInstance);
			std::construct_at(delegateInstance, _object, _method);
		}

		template <class TObject>
		void BindSharedRaw(const std::shared_ptr<const TObject>& _object, TConstMemberFunction<TObject> _method)
		{
			Clear();

			Allocator.Allocate(sizeof(TConstSharedRawDelegateInstance<const TObject>));
			TConstSharedRawDelegateInstance<const TObject>* delegateInstance =
				Allocator.Cast<TConstSharedRawDelegateInstance<const TObject>>();

			JE_ASSERT(delegateInstance);
			std::construct_at(delegateInstance, _object, _method);
		}

		static FDelegate CreateStaticDelegate(TStaticFunction _staticFunctionPtr)
		{
			FDelegate delegate;
			delegate.BindStatic(_staticFunctionPtr);
			return delegate;
		}

		template <class TObject>
		static FDelegate CreateRawDelegate(TObject* _object, TMemberFunction<TObject> _method)
		{
			FDelegate delegate;
			delegate.BindRaw(_object, _method);
			return delegate;
		}

		template <class TObject>
		static FDelegate CreateRawDelegate(const TObject* _object, TConstMemberFunction<TObject> _method)
		{
			FDelegate delegate;
			delegate.BindRaw(_object, _method);
			return delegate;
		}

		template <class TLambda>
		static FDelegate CreateLambdaDelegate(TLambda&& _lambda)
		{
			FDelegate delegate;
			delegate.BindLambda(std::forward<TLambda>(_lambda));
			return delegate;
		}

		template <class TObject>
		static FDelegate CreateSharedRawDelegate(const std::shared_ptr<TObject>& _object, TMemberFunction<TObject> _method)
		{
			FDelegate delegate;
			delegate.BindSharedRaw(_object, _method);
			return delegate;
		}

		template <class TObject>
		static FDelegate CreateSharedRawDelegate(const std::shared_ptr<const TObject>& _object, TConstMemberFunction<TObject> _method)
		{
			FDelegate delegate;
			delegate.BindSharedRaw(_object, _method);
			return delegate;
		}
	};

} // namespace JE
