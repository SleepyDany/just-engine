#pragma once

#include "Assertions/Assert.h"

namespace JE
{
	template <bool Const, class TObject, class TReturn, class... TArgs>
	struct TMemberFunctionPtr;

	template <class TObject, class TReturn, class... TArgs>
	struct TMemberFunctionPtr<true, TObject, TReturn, TArgs...>
	{
		using Type = TReturn (TObject::*)(TArgs...) const;
	};

	template <class TObject, class TReturn, class... TArgs>
	struct TMemberFunctionPtr<false, TObject, TReturn, TArgs...>
	{
		using Type = TReturn (TObject::*)(TArgs...);
	};

	class JE_API FDelegateHandle
	{
		//- Types ------------------------
	public:
		enum EMode
		{
			Empty = 0,
			Generate
		};

		//- Variables --------------------
	public:
		constexpr static uint64 InvalidId = 0ll;

	private:
		static std::atomic<uint64> GlobalId;
		uint64 Id = InvalidId;

		//- Lifecycle --------------------
	public:
		FDelegateHandle(EMode _mode = Empty);
		~FDelegateHandle();

		FDelegateHandle(const FDelegateHandle& _handle) = default;
		FDelegateHandle& operator=(const FDelegateHandle& _handle) = default;
		FDelegateHandle(FDelegateHandle&& _handle) noexcept;
		FDelegateHandle& operator=(FDelegateHandle&& _handle) noexcept;

		bool operator==(const FDelegateHandle& _handle) const;
		bool operator!=(const FDelegateHandle& _handle) const;

		//- Methods ----------------------
	public:
		void Reset();

		bool IsValid() const;

		uint64 GetId() const;

		/** Generate next Id for delegate. Threadsafe. */
		static uint64 GenerateId();
	};

	template <class TReturn, class... TArgs>
	class JE_API IDelegateBase
	{
		//- Lifecycle --------------------
	public:
		IDelegateBase() = default;
		virtual ~IDelegateBase() = default;

		//- Methods ----------------------
	public:
		/** Delegate execution. */
		virtual TReturn Execute(TArgs&&...) const = 0;

		/** Returns owner object for delegate. Works only for Raw and SharedRaw delegates. */
		virtual const void* GetOwner() const noexcept
		{
			return nullptr;
		}
	};

	template <bool bConst, class TObject, class TReturn, class... TArgs>
	class JE_API FRawDelegateBase : public IDelegateBase<TReturn, TArgs...>
	{
		//- Types ------------------------
	private:
		using TMethod = typename TMemberFunctionPtr<bConst, TObject, TReturn, TArgs...>::Type;

		//- Variables --------------------
	protected:
		TObject* ObjectPtr = nullptr;

		TMethod MethodPtr = nullptr;

		//- Lifecycle --------------------
	public:
		explicit FRawDelegateBase(TObject* _object, TMethod _method)
			: IDelegateBase<TReturn, TArgs...>()
			, ObjectPtr(_object)
			, MethodPtr(_method)
		{
			JE_ASSERT(ObjectPtr != nullptr && MethodPtr != nullptr);
		}

		//- Methods ----------------------
	public:
		virtual TReturn Execute(TArgs&&... _args) const final
		{
			JE_ASSERT(ObjectPtr != nullptr && MethodPtr != nullptr);
			return std::invoke(MethodPtr, ObjectPtr, std::forward<TArgs>(_args)...);
		}

		virtual const void* GetOwner() const noexcept final
		{
			return ObjectPtr;
		}
	};

	template <bool bConst, class TObject, class TReturn, class... TArgs>
	class JE_API FSharedRawDelegateBase : public IDelegateBase<TReturn, TArgs...>
	{
		//- Types ------------------------
	private:
		using TMethod = typename TMemberFunctionPtr<bConst, TObject, TReturn, TArgs...>::Type;

		//- Variables --------------------
	protected:
		std::weak_ptr<TObject> WeakObjectPtr = nullptr;

		TMethod MethodPtr = nullptr;

		//- Lifecycle --------------------
	public:
		explicit FSharedRawDelegateBase(const std::shared_ptr<TObject>& _object, TMethod _method)
			: IDelegateBase<TReturn, TArgs...>()
			, WeakObjectPtr(_object)
			, MethodPtr(_method)
		{
			JE_ASSERT(MethodPtr != nullptr);
		}

		//- Methods ----------------------
	public:
		virtual TReturn Execute(TArgs&&... _args) const final
		{
			JE_ASSERT(!WeakObjectPtr.expired());
			std::shared_ptr<TObject> ObjectPtr = WeakObjectPtr.lock();

			JE_ASSERT(MethodPtr != nullptr);
			return std::invoke(MethodPtr, ObjectPtr, std::forward<TArgs>(_args)...);
		}

		virtual const void* GetOwner() const noexcept final
		{
			return WeakObjectPtr.lock().get();
		}
	};

	template <class TLambda, class TReturn, class... TArgs>
	class JE_API FLambdaDelegateBase : public IDelegateBase<TReturn, TArgs...>
	{
		//- Variables --------------------
	private:
		TLambda Lambda;

		//- Lifecycle --------------------
	public:
		explicit FLambdaDelegateBase(TLambda&& _lambda)
			: IDelegateBase<TReturn, TArgs...>()
			, Lambda(std::forward<TLambda>(_lambda))
		{
		}

		//- Methods ----------------------
	public:
		virtual TReturn Execute(TArgs&&... _args) const final
		{
			return std::invoke(Lambda, std::forward<TArgs>(_args)...);
		}
	};

	template <class TReturn, class... TArgs>
	class JE_API FStaticDelegateBase : public IDelegateBase<TReturn, TArgs...>
	{
		//- Types ------------------------
	private:
		using TMethod = TReturn (*)(TArgs...);

		//- Variables --------------------
	private:
		TMethod StaticMethodPtr = nullptr;

		//- Lifecycle --------------------
	public:
		explicit FStaticDelegateBase(TMethod _method)
			: IDelegateBase<TReturn, TArgs...>()
			, StaticMethodPtr(_method)
		{
		}

		//- Methods ----------------------
	public:
		virtual TReturn Execute(TArgs&&... _args) const final
		{
			JE_ASSERT(StaticMethodPtr != nullptr);
			return std::invoke(StaticMethodPtr, std::forward<TArgs>(_args)...);
		}
	};

} // namespace JE
