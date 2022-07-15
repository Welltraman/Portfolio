using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public delegate void ObserverMethod(params object[] pArgs);

public abstract class Observable : MonoBehaviour {
    Dictionary<string, List<ObserverMethod>> mObserverMap = new Dictionary<string, List<ObserverMethod>>();
    public void Subscribe(string pName, ObserverMethod pMethod) {
        if (!mObserverMap.ContainsKey(pName)) {
            mObserverMap.Add(pName, new List<ObserverMethod>());
        }
        mObserverMap[pName].Add(pMethod);
    }
    protected void Notify(string pName, params object[] pArgs) {
        int observerCount = mObserverMap[pName].Count;
        if (observerCount == 0) {
            Debug.LogError(string.Format("No observer registered with name: '{0}'", pName));
            return;
        }
        foreach (ObserverMethod om in mObserverMap[pName]) {
            om(pArgs);
        }
    }
}