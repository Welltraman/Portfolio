using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class PlayButton : MonoBehaviour
{
    public void PlayGame() {
        SceneLoader.OnLoadedMethod olm = GameManager.Instance.StartGame;
        GameManager.Instance.ChangeScene("GameScene", olm);
    }
}
