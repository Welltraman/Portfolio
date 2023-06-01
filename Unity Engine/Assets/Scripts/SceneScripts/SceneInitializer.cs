using UnityEngine;
using UnityEngine.SceneManagement;

public class SceneInitializer : MonoBehaviourPlus {
    public GameObject gameController;
    protected override void Awake() {
        if (!GameManager.instance) Instantiate(gameController);
        base.Awake();
        SceneManager.activeSceneChanged += OnSceneStarted;
    }
    public void LoadScene(string pScene) => SceneManager.LoadScene(pScene);
    public void OnSceneStarted(Scene pReplaced, Scene pNext) {
        SceneManager.activeSceneChanged -= OnSceneStarted;
        if (pNext.name == "ManuScene") GameState.ChangeGameState(GameStateEnum.MENU);
        if (pNext.name == "GameScene") {
            GameState.ChangeGameState(GameStateEnum.ROUND_BEGIN);
            InitializeGameScene();
        }
    }
    public void InitializeGameScene() {
        GameManager.GetSceneComponent<MapFactory>().CreateRandomMap();
        GameManager.GetSceneComponent<CharacterPicker>().Initialize(CharacterFactory.GetCharacters(Settings.instance.startingHand));
    }
}
