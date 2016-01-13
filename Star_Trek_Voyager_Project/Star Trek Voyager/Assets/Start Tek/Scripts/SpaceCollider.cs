using UnityEngine;
using System.Collections;

public class SpaceCollider : MonoBehaviour
{
	void OnTriggerExit2D(Collider2D coll) {
		if (coll.gameObject.tag == "Ship") {
			//GameManager.UpdateScoreOnLoose();
			Application.LoadLevel("LevelChange");			

		}	
	}

}

