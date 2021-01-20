#pragma once

namespace Pine {

	class Window {
	public:
		virtual ~Window() = default;
	private:
		virtual void Init();
	};
		
}