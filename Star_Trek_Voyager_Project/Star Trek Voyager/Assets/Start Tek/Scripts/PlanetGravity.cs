using UnityEngine;
using System.Collections;
public class PlanetGravity : MonoBehaviour
{
	private Transform ship;
	private Transform planet;
	private static Transform currentPlanet;
	//private Vector3 worldPlanetPosition;
	private Rigidbody2D planetbody;
	private float gravitationalForce = 15.0f;
	private float radius = 0.0f;
	private float mass = 0.0f;
	public bool isVisited = false;
	void Start()
	{
		planet = transform.parent.transform;
		planetbody = planet.GetComponent<Rigidbody2D> ();
		mass = planetbody.mass;
		radius = transform.GetComponent<CircleCollider2D> ().radius;	
	}

	void OnTriggerStay2D(Collider2D coll) {
		if (coll.gameObject.tag == "Ship") {

			ship = coll.gameObject.transform;
			if(planet != currentPlanet && !ship.GetComponent<HingeJoint2D>().enabled) 
			{
				Ship.isMoving = false;

				Vector3 direction = planet.position - ship.transform.position;
				float gravitationalStrength = (float)((gravitationalForce * mass ) / (direction.sqrMagnitude));
				Vector3 gravity = (direction.normalized * gravitationalStrength);

				ship.GetComponent<Rigidbody2D>().AddForce(gravity);	

			}
		}	
	}

	public void SetCurrentPlanet(Transform p)
	{
		currentPlanet = p;
	}

	

}