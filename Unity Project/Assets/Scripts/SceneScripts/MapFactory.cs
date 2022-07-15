using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class MapFactory : MonoBehaviourPlus
{
    public List<GameObject> maps = new List<GameObject>();
    private GameObject mCurrentMap;
    public TileManager GetRandomMap() {
        if (maps.Count == 0) return null;
        GameObject rawmap = maps[Random.Range(0, maps.Count)];
        rawmap.AddComponent<TileManager>();
        if (mCurrentMap != null) mCurrentMap.GetComponent<TileManager>().Clear();
        mCurrentMap = Instantiate(rawmap);
        return mCurrentMap.GetComponent<TileManager>();
    }
    public TileManager GetCurrentMap() {
        return mCurrentMap.GetComponent<TileManager>();
    }
}
