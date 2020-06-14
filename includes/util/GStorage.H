#pragma once

#include "util/singleton.h"
#include "util/LStorage.h"

namespace Oper 
{
	template<typename T>
	class GStorage final : public Singleton<GStorage<T>>, public LStorage<T> {
		friend class Singleton<GStorage<T>>;
	public:
		static GStorage<T> * GetInstance() {
			return Singleton<GStorage<T>>::GetInstance();
		}
	private:
		GStorage() = default;
		~GStorage() = default;
	};
}