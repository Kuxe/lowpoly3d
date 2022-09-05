
#include <cstdint>
#include <queue>

// WIP: This class is not finished / tested. I just commited it quickly
// from a godbolt I toyed around with, since I know I am having some local changes
// on another machine (laptop) that resembles this... Might use this instead of my other local changes!

// A ResourceManager owns resources.
// Each loaded resource is associated with a copyable Handle.
// The handle can be used to retrieve the resource from the ResourceManager.
// TResource is the type of the object that is retrieved and TResourceDefinition is the type
// that contains all data needed for loading the resource, e.g a path to a file on disk.

struct ResourceHandle
{
	std::uint16_t handle = 0;
};

template<typename TResourceTypes>
class ResourceManager
{
public:

	// This is the resource itself that is to be used by the program.
	// It needs to be assignable via operator=(Resource const&).
	using Resource = typename TResourceTypes::Resource;
	// This type contains data needed for loading the resource, e.g a path to a file.
	using ResourceDefinition = typename TResourceTypes::ResourceDefiniton;
	// This type should have one static method ResourceLoader::load(ResourceDefinition const& def),
	// which loads the resource (whatever that entails)
	using ResourceLoader = typename TResourceTypes::ResourceLoader;
	// This type should have one static method ResourceLoader::unload(Resource const& res),
	// which unloads the resource (whatever that entails)
	using ResourceUnloader = typename TResourceTypes::ResourceUnloader;
	// A lightweight handle to the resource. Needs to be integer type.
	using ResourceHandle = typename TResourceTypes::ResourceHandle;

	// Loads a resource.
	ResourceHandle load(ResourceDefinition iResourceDefinition)
	{
		// Check if there are any recycled handles, if so, reuse
		if(!mFreedHandles.empty())
		{
			ResourceHandle recycledHandle = mFreedHandles.back();
			mFreedHandles.pop_back();
			mResources[recycledHandle.handle] = ResourceLoader::load(iResourceDefinition);
			return recycledHandle;
		}

		mResources.emplace_back(ResourceLoader::load(iResourceDefinition));
		return ResourceHandle{nextHandle++};
	};

	Resource& get(ResourceHandle iHandle)
	{
		return mResources[iHandle.handle];
	}

	// Unloads a resource.
	void unload(ResourceHandle iHandle)
	{
		ResourceUnloader::unload(get(iHandle));
		mFreedHandles.emplace_back(iHandle);
	}

private:
	std::vector<Resource> mResources;
	std::vector<ResourceHandle> mFreedHandles;
	std::uint16_t nextHandle = 1;
};