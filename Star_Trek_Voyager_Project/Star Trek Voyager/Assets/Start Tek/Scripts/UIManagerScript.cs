using UnityEngine;
using System.Collections;

public class UIManagerScript : MonoBehaviour {
	private static int level;
	
	public void StartGame() {
		level = 1;
		Application.LoadLevel("Stage 1");

	}
	public void ReplayLevel()
	{
		Application.UnloadLevel("Stage " +level.ToString());
		Application.LoadLevel("Stage " +level.ToString());	
	}

	public void NextLevel()
	{
		level = level + 1;
		if (level > 4) {
			Application.LoadLevel("GameOver");
		}
		else
			Application.LoadLevel("Stage " +level.ToString());
	}

	public void PreviousLevel()
	{
		level = level - 1;
		GameManager.UpdateScoreOnLoose();
		Application.LoadLevel("Stage " +level.ToString());
	}

	public void Exit()
	{
		//if (UnityEditor.EditorApplication != null)
			UnityEditor.EditorApplication.isPlaying = false;
		Application.Quit ();
		//Application.UnloadLevel ();
	}

	public void RetunMenu()
	{
		Application.UnloadLevel("Stage " +level.ToString());
		Application.LoadLevel ("LevelChange");
	}

	public static void UpdateLevel()
	{
		level++;
	}

	public static int GetLevel()
	{
		return level;
	}
}
