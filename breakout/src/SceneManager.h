#pragma once
namespace SceneManager
{
	//Saves every scene in the game
	enum Scene
	{
		None,
		Gameplay,
		Result
	};

	//changes the current scene
	void SetCurrentScene(Scene scene);
	//returns the current scene
	Scene GetCurrentScene();
}
