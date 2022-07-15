using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.SceneManagement;

public class SceneLoader : MonoBehaviour
{
    public delegate void OnLoadedMethod();

    public void Load(string pSceneName, OnLoadedMethod pMethod)
    {
        StartCoroutine(LoadAsyncScene(pSceneName, pMethod));
    }

    IEnumerator LoadAsyncScene(string pSceneName, OnLoadedMethod pMethod = null)
    {
        AsyncOperation asyncLoad = SceneManager.LoadSceneAsync(pSceneName);
        while (!asyncLoad.isDone)
        {
            yield return null;
        }
        if (pMethod != null) pMethod();
    }
}
