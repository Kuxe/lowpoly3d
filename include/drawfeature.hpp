#ifndef DRAWFEATURE_HPP
#define DRAWFEATURE_HPP

#include <memory>
#include <vector>

namespace lowpoly3d
{
	struct IDrawFeature;
	class Renderer;

	struct IDrawCall
	{
		virtual void draw() const = 0;
	};

	// DrawFeatureTarget can set various properties.
	// This tweaks the drawing behaviour of the
	class DrawFeatureTarget final
	{
	public:
		DrawFeatureTarget();
		~DrawFeatureTarget();
		DrawFeatureTarget(DrawFeatureTarget const& iOther);
		DrawFeatureTarget(DrawFeatureTarget&& iOther);
		DrawFeatureTarget& operator=(DrawFeatureTarget iOther);
		DrawFeatureTarget& operator=(DrawFeatureTarget&& iOther);

		void draw(IDrawCall const& iDrawCall, Renderer& iRenderer) const;

		DrawFeatureTarget& setNoFaceCull();
	private:
		std::vector<std::unique_ptr<IDrawFeature>> mDrawFeatures;
		friend void swap(DrawFeatureTarget&, DrawFeatureTarget&);
	};

	template<typename TDrawCall>
	std::unique_ptr<IDrawCall> prepareDrawCall(TDrawCall&& iDrawCall)
	{
		struct DrawCall : public IDrawCall
		{
			DrawCall(TDrawCall&& iDrawCall)
				: mDrawCall(std::forward<TDrawCall>(iDrawCall))
			{ }
			
			void draw() const override
			{
				mDrawCall();
			}

			TDrawCall mDrawCall;
		};
		return std::make_unique<DrawCall>(std::forward<TDrawCall>(iDrawCall));
	}

	void swap(DrawFeatureTarget& lhs, DrawFeatureTarget& rhs);

} // End of namespace lowpoly3d

#endif // DRAWFEATURE_HPP