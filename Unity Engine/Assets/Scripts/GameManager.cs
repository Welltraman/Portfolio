using System;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.SceneManagement;

public class GameManager : MonoBehaviour {
    private static GameManager _instance;
    public static GameManager instance { get => _instance; }
    void Awake() {
        if (_instance != null && _instance != this) Destroy(this); else _instance = this;
        DontDestroyOnLoad(gameObject);
    }
    /// Scene Component Dictionary ================================================================
    private Dictionary<string, Component> mSceneComponents = new Dictionary<string, Component>();
    void Start() {
        SceneManager.sceneUnloaded += (Scene _) => mSceneComponents.Clear();
    }
    public void AddSceneComponent(Component pComp) {
        string name = pComp.GetType().Name;
        mSceneComponents[name] = pComp;
    }
    public static T GetSceneComponent<T>() {
        string name = typeof(T).Name;
        return (T)Convert.ChangeType(instance.mSceneComponents[name], typeof(T));
    }
    public void RemoveSceneComponent(Component pComp) {
        string name = pComp.GetType().Name;
        mSceneComponents.Remove(name);
    }
    /// Public Methods ============================================================================
    public void Quit() => Application.Quit();
}