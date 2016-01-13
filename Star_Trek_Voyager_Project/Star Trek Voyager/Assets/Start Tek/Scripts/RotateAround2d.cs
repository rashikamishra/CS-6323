using UnityEngine;
using System.Collections;

public class RotateAround2d : MonoBehaviour {

    public float speed;
    public Rigidbody2D target;

    void Start() {
		if (target == null) 
		{
            target = GetComponent<Rigidbody2D>();
            Debug.Log ("RotateAround target not specified. Defaulting to parent GameObject");
		}
	}

    void FixedUpdate()
    {
		//if(transform.tag == "Planet")
        	target.MoveRotation(target.rotation + speed * Time.fixedDeltaTime);

    }
}
