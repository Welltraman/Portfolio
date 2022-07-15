using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.EventSystems;
using UnityEngine.UI;

public class CharacterPicker : MonoBehaviourPlus {
	public Color disabledColor;
	private float mImageHeight = 100f, mImageWidth = 100f;
	private Dictionary<Vector3Int, GameObject> mDeployedCharacterMap = new Dictionary<Vector3Int, GameObject>();
	public void Initialize(List<GameObject> pCharacters) {
		if (pCharacters != null) {
			GetComponent<RectTransform>().sizeDelta = new Vector2(pCharacters.Count * mImageWidth, mImageHeight);
			for (int i = 0; i < pCharacters.Count; i++) AddImageAtPicker(pCharacters[i]);
		}
	}
	private void AddImageAtPicker(GameObject pCharacterRaw) {
		GameObject obj = new GameObject();
		obj.name = pCharacterRaw.name;
		obj.AddComponent<CanvasGroup>();
		obj.AddComponent<CharacterDragHandler>();
		CharacterBase character = pCharacterRaw.GetComponent<CharacterBase>();
		obj.AddComponent(character.GetType());
		Image img = obj.AddComponent<Image>();
		img.sprite = pCharacterRaw.GetComponent<SpriteRenderer>().sprite;
		RectTransform rt = obj.GetComponent<RectTransform>();
		float anchorX = 1f / (Settings.Instance.startingHand - 1) * transform.childCount;
		rt.anchorMin = new Vector2(anchorX, 0.5f);
		rt.anchorMax = new Vector2(anchorX, 0.5f);
		rt.pivot = new Vector2(anchorX, 0.5f);
		obj.transform.SetParent(this.transform, false);
	}
	public GameObject AddDisabledImage(GameObject pCharacterImage) {
		GameObject obj = new GameObject();
		obj.name = pCharacterImage.name + " - disabled";
		DisabledCharacterImage script = obj.AddComponent<DisabledCharacterImage>();
		script.characterImage = pCharacterImage;
		Image img = obj.AddComponent<Image>();
		img.sprite = pCharacterImage.GetComponent<Image>().sprite;
		img.color = disabledColor;
		RectTransform rt = obj.GetComponent<RectTransform>();
		RectTransform rt_base = pCharacterImage.GetComponent<RectTransform>();
		rt.anchorMin = rt_base.anchorMin;
		rt.anchorMax = rt_base.anchorMax;
		rt.pivot = rt_base.pivot;
		obj.transform.SetParent(this.transform, false);
		return obj;
	}
	public void BringBackImage(GameObject pCharacterImage) {
		Destroy(pCharacterImage.GetComponent<CharacterDragHandler>().disabledImage);
		pCharacterImage.GetComponent<RectTransform>().anchoredPosition = Vector3.zero;
	}
	public void DeployCharacter(Vector3Int pPos, GameObject pCharacterImage) {
		mDeployedCharacterMap.Add(pPos, pCharacterImage.GetComponent<CharacterDragHandler>().disabledImage);
		pCharacterImage.SetActive(false);
	}
	public GameObject UnDeployCharacter(Vector3Int pPos) {
		GameObject img = mDeployedCharacterMap[pPos].GetComponent<DisabledCharacterImage>().characterImage;
		mDeployedCharacterMap.Remove(pPos);
		img.SetActive(true);
		return img;
	}
}

public class CharacterDragHandler : MonoBehaviour, IPointerDownHandler, IBeginDragHandler, IDragHandler, IEndDragHandler {
	public GameObject disabledImage;
	private CanvasGroup mCanvasGroup;
	void Awake() {
		mCanvasGroup = GetComponent<CanvasGroup>();
	}
	public void OnPointerDown(PointerEventData pData) {
		if (disabledImage == null)
			disabledImage = transform.parent.GetComponent<CharacterPicker>().AddDisabledImage(this.gameObject);
		pData.pointerDrag = gameObject;
		transform.parent.parent.GetChild(1).GetComponent<CharacterDescription>().AddDescription(this.gameObject);
	}
	public void OnBeginDrag(PointerEventData pData) {
		mCanvasGroup.alpha = 0.6f;
		mCanvasGroup.blocksRaycasts = false;
	}
	public void OnDrag(PointerEventData pData) {
		GetComponent<RectTransform>().anchoredPosition += pData.delta;
	}
	public void OnEndDrag(PointerEventData pData) {
		mCanvasGroup.alpha = 1;
		mCanvasGroup.blocksRaycasts = true;
	}
}

public class DisabledCharacterImage : MonoBehaviour {
	public GameObject characterImage;
}