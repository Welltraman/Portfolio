using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class RunButton : MonoBehaviour
{
    public void Run() {
        MapFactory mf = GameManager.Instance.GetSceneObject("MapFactory").GetComponent<MapFactory>();
        TileManager map = mf.GetCurrentMap();
        map.ChangeEnemySpawnShade();
        //Initialize();
    }
    void Initialize() {
        foreach (Transform field in transform) {
            foreach (Transform character in field) {
                Effects effect = character.GetComponent<CharacterBase>().Initialize();
            }
        }
    }
}
