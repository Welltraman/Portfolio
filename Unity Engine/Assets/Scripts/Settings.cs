using UnityEngine;

public class Settings : MonoBehaviour {
    // configs ------------------------------------------------------------------------------------
    public int startingHand = 5;
    public float simulationSpeedMultiplier = 1f;
    // --------------------------------------------------------------------------------------------
    static private Settings _instance;
    static public Settings instance { get => _instance; }
    void Awake() { if (_instance != null && _instance != this) Destroy(this); else _instance = this; }
}
