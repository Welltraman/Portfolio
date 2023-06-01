using UnityEngine;
using UnityEngine.EventSystems;
using UnityEngine.Tilemaps;

public class CharacterDeployer : MonoBehaviourPlus, IDropHandler, IPointerDownHandler, IDragHandler {
    private TileManager mTileManager;
    private CharacterPicker mPicker;
    protected override void Awake() {
        base.Awake();
        mTileManager = transform.parent.GetComponent<TileManager>();
        mPicker = GameManager.GetSceneComponent<CharacterPicker>();
    }
    protected override void OnGameState(GameStateEnum pState) {
        switch (pState) {
            case GameStateEnum.SIMULATE:
                mLastUndeployedCharacterImg = null;
                break;
        }
    }
    // Drop & pick up =============================================================================
    private GameObject mLastUndeployedCharacterImg;
    public void OnDrop(PointerEventData pData) {
        if (!pData.pointerDrag) return;
        ExecuteEvents.Execute<IPointerUpHandler>(mLastUndeployedCharacterImg, pData, ExecuteEvents.pointerUpHandler);
        RaycastHit2D hit = Physics2D.Raycast(Camera.main.ScreenToWorldPoint(Input.mousePosition), Vector2.zero);
        if (hit && hit.collider.gameObject == gameObject) {
            Vector3Int pos = GetComponent<Tilemap>().WorldToCell(hit.point);
            if (mTileManager.DeployPlayerCharacter(pos, pData.pointerDrag)) {
                mPicker.BringBackImage(pData.pointerDrag, false);
                return;
            }
        }
        mPicker.BringBackImage(pData.pointerDrag);
    }
    public void OnPointerDown(PointerEventData pData) {
        RaycastHit2D hit = Physics2D.Raycast(Camera.main.ScreenToWorldPoint(Input.mousePosition), Vector2.zero);
        if (GameState.currentGameState == GameStateEnum.ROUND_BEGIN && hit) {
            Vector3Int pos = GetComponent<Tilemap>().WorldToCell(hit.point);
            GameObject drag;
            if (mTileManager.RemoveCharacterAt(pos, out drag)) {
                mPicker.EnableCharacterDrag(drag, pData);
                mLastUndeployedCharacterImg = drag;
            }
        }
    }
    public void OnDrag(PointerEventData pData) {
        pData.pointerDrag = mLastUndeployedCharacterImg;
    }
}
