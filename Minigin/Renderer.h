#pragma once
#include "Singleton.h"

struct SDL_Window;
struct SDL_Renderer;

namespace dae
{
	class Texture2D;
	/**
	 * Simple RAII wrapper for the SDL renderer
	 */
	class Renderer final : public Singleton<Renderer>
	{
	public:
		void Init(SDL_Window* window);
		void Render() const;
		void Destroy();

		void RenderTexture(const Texture2D& texture, const float x, const  float y, const  float scale = 1.f) const;
		void RenderTexture(const Texture2D& texture, const float x, const  float y, const  float width, const  float height) const;
		
		void RenderTexture(const Texture2D& texture, 
			const int dstX,
			const int dstY,
			const int dstWidth,
			const int dstHeight,
			const int srcX,
			const int srcY,
			const int srcWidth,
			const int srcHeight) const;

		SDL_Renderer* GetSDLRenderer() const { return m_Renderer; }
	private:
		SDL_Renderer* m_Renderer{};
	};
}

