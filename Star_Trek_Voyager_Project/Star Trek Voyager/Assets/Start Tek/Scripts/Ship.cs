using UnityEngine;
using System.Collections;
public class Ship : MonoBehaviour
{
	//public static int level;
	private GameObject currentPlanet; 
	public Transform sourceplanet;
	private Rigidbody2D rigidbody2;
	public static bool isMoving = false;
	private float speed = 10.0f;
	private float step = 0.0f;
	Vector3 movement;
	GameObject[] planets;
	private int score = 0;
	void Start()
	{
		planets = GameObject.FindGameObjectsWithTag("Planet");
		rigidbody2 = transform.GetComponent<Rigidbody2D> ();

	}

	void OnCollisionEnter2D(Collision2D coll) {
		if (coll.gameObject.tag == "Planet") {
				currentPlanet = coll.gameObject;
				PlanetGravity pg = new PlanetGravity();
				pg.SetCurrentPlanet(currentPlanet.transform);
				isMoving = false;
				HingeJoint2D joint = transform.GetComponent<HingeJoint2D>();
				if(!joint.enabled)
				{
					joint.connectedBody = currentPlanet.transform.GetComponent<Rigidbody2D>();
					Vector2 anchor = new Vector2(coll.contacts[0].point.x,coll.contacts[0].point.y);;
					joint.connectedAnchor = currentPlanet.transform.InverseTransformPoint(anchor);
					transform.up = coll.contacts[0].normal;
					joint.enabled = true;
					Transform range = currentPlanet.transform.FindChild("GravityRange");
					if(range != null)
					{
						PlanetGravity gravity = range.GetComponent<PlanetGravity>();
						if(gravity != null)
							if(!gravity.isVisited)
							{
								score++;
								gravity.isVisited = true;
							}
					}
					

			}
			if(currentPlanet.name.Equals("final"))
			{
				GameManager.UpdateScoreOnWin(score);
				Application.LoadLevel("NextLevelChange");

			}
			if(currentPlanet.name.Equals("finall"))
			{
				GameManager.UpdateScoreOnWin(score);
				Application.LoadLevel("GameOver");
				
			}
			if(currentPlanet.name.Equals("Blackhole"))
			{
				Application.LoadLevel("LevelChange");
			}
		}	
	}

	void MoveToTarget() {
		if (isMoving) 
		{
			transform.up = movement;
			step = speed * Time.deltaTime;
			rigidbody2.AddForce(movement * step ,ForceMode2D.Impulse);
		}		
	}

	void FixedUpdate () 
	{
		if (Input.GetMouseButton (0)) 
		{
			movement = transform.up.normalized;
			isMoving = true;
			//MoveToTarget ();
			transform.GetComponent<HingeJoint2D>().enabled = false;

		}	
		MoveToTarget ();
	}
}