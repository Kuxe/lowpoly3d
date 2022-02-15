#include "drawfeature.hpp"

#include <cstring>
#include "renderer.hpp"

#include <glbinding/gl/gl.h>

#include <algorithm>

using namespace gl;

namespace lowpoly3d
{
	struct IGLStateRestorer
	{
		virtual void restore() const = 0;
	};

	std::unique_ptr<IGLStateRestorer> createNullRestorer()
	{
		struct NullRestorer : public IGLStateRestorer {
			void restore() const override {}
		};
		return std::make_unique<NullRestorer>();
	}

	struct IDrawFeature
	{
		virtual ~IDrawFeature() = default;
		virtual IDrawFeature* clone() const = 0;
		virtual std::unique_ptr<IGLStateRestorer> setup(DrawFeatureTarget const& iTarget, Renderer& iRenderer) const = 0;
	};

	template<typename TDerived>
	struct DrawFeatureCRTP : public IDrawFeature
	{
		DrawFeatureCRTP<TDerived>* clone() const override
		{
			return new TDerived();
		}
	};

	DrawFeatureTarget::DrawFeatureTarget() = default;
	DrawFeatureTarget::~DrawFeatureTarget() = default;

	DrawFeatureTarget::DrawFeatureTarget(DrawFeatureTarget const& iOther)
		: mDrawFeatures([&](){
				decltype(mDrawFeatures) ret;
				ret.reserve(iOther.mDrawFeatures.size());
				for(auto const& elem : iOther.mDrawFeatures)
				{
					ret.emplace_back(elem->clone());
				}
				return ret;
		}())
	{	}

	DrawFeatureTarget::DrawFeatureTarget(DrawFeatureTarget&& iOther)
		: mDrawFeatures(std::move(iOther.mDrawFeatures))
	{ }

	DrawFeatureTarget& DrawFeatureTarget::operator=(DrawFeatureTarget iOther)
	{
		swap(*this, iOther);
		return *this;
	}

	DrawFeatureTarget& DrawFeatureTarget::operator=(DrawFeatureTarget&& iOther)
	{
		if(this != &iOther)
		{
			mDrawFeatures = std::move(iOther.mDrawFeatures);
		}
		return *this;
	}

	void DrawFeatureTarget::draw(IDrawCall const& iDrawCall, Renderer& iRenderer) const
	{
		std::vector<std::unique_ptr<IGLStateRestorer>> restorers;
		restorers.reserve(mDrawFeatures.size());
		transform(begin(mDrawFeatures), end(mDrawFeatures), back_inserter(restorers), [this, &iRenderer](auto const& iFeature){
			return iFeature->setup(*this, iRenderer);
		});

		iDrawCall.draw();

		for_each(begin(restorers), end(restorers), [](auto const& iRestorer){ iRestorer->restore(); });
	}

	DrawFeatureTarget& DrawFeatureTarget::setNoFaceCull()
	{
		class DrawFeatureNoFaceCull : public DrawFeatureCRTP<DrawFeatureNoFaceCull>
		{
		public:
			std::unique_ptr<IGLStateRestorer> setup(DrawFeatureTarget const& iTarget, Renderer& iRenderer) const override
			{
				if(glIsEnabled(GL_CULL_FACE))
				{
					glDisable(GL_CULL_FACE);

					struct GLStateRestorer : public IGLStateRestorer
					{
						void restore() const override
						{
							glEnable(GL_CULL_FACE);
						}
					};

					return std::make_unique<GLStateRestorer>();
				}
				return createNullRestorer();
			}
		};

		mDrawFeatures.emplace_back(std::make_unique<DrawFeatureNoFaceCull>());
		return *this;
	}

	void swap(DrawFeatureTarget& lhs, DrawFeatureTarget& rhs)
	{
		swap(lhs.mDrawFeatures, rhs.mDrawFeatures);
	}

} // End of namespace lowpoly3d