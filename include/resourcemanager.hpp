
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

template<typename TResourceTypes>
class ResourceManager
{
public:

	// This is the resource itself that is to be used by the program.
	// It needs to be assignable via operator=(Resource const&).
	using Data = typename TResourceTypes::Data;
	// This type contains data needed for loading the resource, e.g a path to a file.
	using Definition = typename TResourceTypes::Definition;
	// This type should have one static method ResourceLoader::load(ResourceDefinition const& def),
	// which loads the resource (whatever that entails)
	using Loader = typename TResourceTypes::Loader;
	// This type should have one static method ResourceLoader::unload(Resource const& res),
	// which unloads the resource (whatever that entails)
	using Unloader = typename TResourceTypes::Unloader;
	// A lightweight handle to the resource. Needs to be integer type.
	using Handle = typename TResourceTypes::Handle;

	// Loads a resource.
	Handle load(Definition iResourceDefinition)
	{
		// Check if there are any recycled handles, if so, reuse
		if(!mFreedHandles.empty())
		{
			Handle recycledHandle = mFreedHandles.back();
			mFreedHandles.pop_back();
			mDatas[recycledHandle.handle] = Loader::load(iResourceDefinition);
			return recycledHandle;
		}

		mDatas.emplace_back(Loader::load(iResourceDefinition));
		return Handle{nextHandle++};
	};

	Data& get(Handle iHandle)
	{
		return mDatas[iHandle.handle-INITIAL_HANDLE];
	}

	// Unloads a resource.
	void unload(Handle iHandle)
	{
		Unloader::unload(get(iHandle));
		mFreedHandles.emplace_back(iHandle);
	}

private:
	std::vector<Data> mDatas;
	std::vector<Handle> mFreedHandles;

	// Handle starts at 1, because 0 is reserved for "bad" handle.
	// Vector of datas is 0-indexed, so INITIAL_HANDLE is subtracted
	// whenever a handle is used as index into data-vector.
	static constexpr Handle INITIAL_HANDLE = 1;
	Handle nextHandle = INITIAL_HANDLE;
};