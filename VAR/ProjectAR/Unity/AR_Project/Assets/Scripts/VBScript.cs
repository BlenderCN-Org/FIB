using System.Collections;
using UnityEngine;
using Vuforia;

public class VBScript : MonoBehaviour, IVirtualButtonEventHandler {

	private GameObject VBButtonObject;
	private GameObject Cat;

	// Use this for initialization
	void Start () {

		VBButtonObject = GameObject.Find ("actionbutton");
		VBButtonObject.GetComponent<VirtualButtonBehaviour> ().RegisterEventHandler (this);
		Cat = GameObject.Find ("Cat");

	}
	

	public void OnButtonPressed (VirtualButtonAbstractBehaviour vb){
	
		Debug.Log ("pressed");
		//Cat.transform.Rotate (new Vector3 (0, Time.deltaTime * 1000, 0));
		Cat.GetComponent<Animation> ().Play (); 

	}


	public void OnButtonReleased( VirtualButtonAbstractBehaviour vb ) { 

		Debug.Log ("unpressed");
		Cat.GetComponent<Animation> ().Stop (); 

	}

}