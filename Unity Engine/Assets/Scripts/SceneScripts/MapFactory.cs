using System.Collections.Generic;
using UnityEngine;

public class MapFactory : MonoBehaviourPlus {
    public List<GameObject> maps = new List<GameObject>();
    private GameObject mCurrentMap;
    public TileManager CreateRandomMap() {
        if (maps.Count == 0) return null;
        if (mCurrentMap != null) Destroy(mCurrentMap);
        GameObject mapRaw = maps[Random.Range(0, maps.Count)];
        mCurrentMap = Instantiate(mapRaw);
        return mCurrentMap.GetComponent<TileManager>();
    }
    public TileManager GetCurrentMap() => mCurrentMap.GetComponent<TileManager>();
}
