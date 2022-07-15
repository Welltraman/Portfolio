using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.EventSystems;
using UnityEngine.Tilemaps;

public class CharacterDeployer : MonoBehaviourPlus, IDropHandler, IPointerDownHandler
{
    private CharacterPicker mPicker;
    private TileManager mTileManager;
    public void Initialize(TileManager pTileManager) {
        mPicker = GameManager.Instance.GetSceneObject("CharacterPicker").GetComponent<CharacterPicker>();
        mTileManager = pTileManager;
    }
    public void OnDrop(PointerEventData pData) {
        if (pData.pointerDrag != null) {
            RaycastHit2D hit = Physics2D.Raycast(Camera.main.ScreenToWorldPoint(Input.mousePosition), Vector2.zero);
            if (hit && hit.collider.name == "PlayerSpawn") {
                Vector3Int pos = hit.collider.GetComponent<Tilemap>().WorldToCell(hit.point);
                if (mTileManager.IsPlayerPosNotTaken(pos)) {
                    pData.pointerDrag.GetComponent<CanvasGroup>().blocksRaycasts = true;
                    CharacterFactory cf = GameManager.Instance.GetComponent<CharacterFactory>();
                    GameObject characterRaw = cf.GetCharacter(pData.pointerDrag.name);
                    mTileManager.AddPlayerCharacter(characterRaw, pos);
                    mPicker.DeployCharacter(pos, pData.pointerDrag);
                }
                else mPicker.BringBackImage(pData.pointerDrag);
            }
            else mPicker.BringBackImage(pData.pointerDrag);
        }
    }
    public void OnPointerDown(PointerEventData pData) {
        RaycastHit2D hit = Physics2D.Raycast(Camera.main.ScreenToWorldPoint(Input.mousePosition), Vector2.zero);
        if (hit.collider != null && hit.collider.name == "PlayerSpawn") {
            Vector3Int pos = hit.collider.GetComponent<Tilemap>().WorldToCell(hit.point);
            if (!mTileManager.IsPlayerPosNotTaken(pos)) {
                GameObject img = mPicker.UnDeployCharacter(pos);
                pData.pointerDrag = img;
                ExecuteEvents.Execute<IDragHandler>(img, pData, ExecuteEvents.dragHandler);
                mTileManager.RemovePlayerAt(pos);
                transform.GetChild(1).GetComponent<CharacterDescription>().AddDescription(img);
            }
        }
    }
}
