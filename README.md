lowpoly3d
==============
A 3D engine tailored towards low poly graphics. It is a hobby project :).

# Short self-contained example
... soon

# Two important interfaces
There are two interfaces which you must implement to use lowpoly3d. The RenderQuerier interface specifies what you must send to the renderer in order for it to render your game. The LowpolyInput interface specifies what the renderer might send back to you (keypresses for example).

## The RenderQuerier interface
RenderQuerier is the link between your game and lowpoly3d. RenderQuerier is a C++ interface which makes the renderer in lowpoly3d capable of rendering your scene. You must realize the RenderQuerier interface. The RenderQuerier interface should provide information about your scenery (=where are what objects) and other miscellaneous data such as position of the sun.

## LowpolyInput
The window created by lowpoly3d captures input from keyboard and mouse if the window is active. Therefore there must be some channel where data can move from the renderer to your game. The LowpolyInput interface serves as this channel. LowpolyInput is an interface which demands of you to implement some methods such as "onMouse" and "onKeyPress". These methods are called by lowpoly3d renderer whenver it receives whatever event you provoked.

# Two important structs
In addition there are two important structs which you should embrace to get going with lowpoly3d.
## RenderData
A RenderData is a wrapper around data which shold answer the "where are what objects" question (posed under The RenderQuerier interface!). If your game has a hero, the lowpoly renderer only cares about where the hero is located (=model-matrix) and what he looks like (=3d model). RenderData simply specifies the format of entities/enemies/objects which lowpoly3d renderer expects from your game.

## ModelData
ModelData represents your 3d model. It consists of a vector of N vertices, another vector of N vertex colors and a vector of M triangles, where a triangle in this context is three indices used for indexed drawing. The lowpoly3d renderer can load and render models on this format. When you load a model, you receive an integer handle to the model. This integer handle should be copied to the `int vertexArray` member of RenderData. Then, if you return this RenderData in the `virtual const std::vector<RenderData>& getRenderDatas() const = 0;` method from RenderQuerier interface, it will be rendered by the lowpoly3d renderer using the model you loaded.
