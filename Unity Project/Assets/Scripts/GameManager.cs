using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class GameManager : MonoBehaviour
{
    private static GameManager instance;
    public static GameManager Instance { get { return instance; } }
    void Awake() {
        DontDestroyOnLoad(gameObject);
        if (instance != null && instance != this) Destroy(this);
        else instance = this;
    }
    /// Scene Object Dictionary ================================================================
    private Dictionary<string, GameObject> mSceneObjects = new Dictionary<string, GameObject>();
    public void AddSceneObject(string pName, GameObject pObj) {
        if (mSceneObjects.ContainsKey(pName)) return;
        mSceneObjects.Add(pName, pObj);
    }
    public GameObject GetSceneObject(string pName) {
        return mSceneObjects[pName];
    }
    public void ClearSceneObjects() {
        mSceneObjects.Clear();
    }
    /// Public Methods =========================================================================
    public void StartGame() {
        MapFactory mf = GetSceneObject("MapFactory").GetComponent<MapFactory>();
        TileManager map = mf.GetRandomMap();
        CharacterDeployer cd = GetSceneObject("UI").GetComponent<CharacterDeployer>();
        cd.Initialize(map);
        CharacterFactory cf = GetComponent<CharacterFactory>();
        CharacterPicker cp = GetSceneObject("CharacterPicker").GetComponent<CharacterPicker>();
        cp.Initialize(cf.GetRandomCharacters(Settings.Instance.startingHand));
        map.SpawnEnemy(cf.GetRandomCharacters(Settings.Instance.startingHand));
    }
    public void ChangeScene(string pSceneName, SceneLoader.OnLoadedMethod pMethod) {
        GetComponent<SceneLoader>().Load(pSceneName, pMethod);
    }
    public void Quit() {
        Application.Quit();
    }
}