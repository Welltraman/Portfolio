using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class MonoBehaviourPlus : MonoBehaviour
{
    void Awake() {
        if (GameManager.Instance != null)
            GameManager.Instance.AddSceneObject(this.name, this.gameObject);
    }
}
