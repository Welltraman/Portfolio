using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class Settings : MonoBehaviour
{
    // configs -------------------------
    public int startingHand = 5;
    // ---------------------------------
    static private Settings instance;
    static public Settings Instance { get { return instance; } }
    void Awake() {
        if (instance != null && instance != this) Destroy(this);
        else instance = this;
    }
}
