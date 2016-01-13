using UnityEngine;
using System.Collections;

public class GameManager : MonoBehaviour
{

	private static int score = 0;
	private static int latestlevelscore = 0;
	private static bool Win = false;
	public static  void UpdateScoreOnWin(int s)
	{
		Win = true;
		score = score + s;
		latestlevelscore = s;
	}

	public static  void UpdateScoreOnLoose()
	{
		Win = false;
		score = score - latestlevelscore;
	}

	public static void UpdateScoreonReplay()
	{
		if (Win) {
			score = score - 2 * latestlevelscore;
		}
	}

	public static int GetScore (){return score;}

	// Update is called once per frame
	void Update ()
	{
		transform.GetComponent<TextMesh> ().text = score.ToString();
	
	}
}

