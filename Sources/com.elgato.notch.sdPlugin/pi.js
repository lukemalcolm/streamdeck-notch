let websocket = null,
    uuid = null,
    actionInfo = {};

function connectElgatoStreamDeckSocket(inPort, inPropertyInspectorUUID, inRegisterEvent, inInfo, inActionInfo) {

    uuid = inPropertyInspectorUUID;
    actionInfo = JSON.parse(inActionInfo);

    websocket = new WebSocket('ws://localhost:' + inPort);

    websocket.onopen = function()
    {
        // WebSocket is connected, register the Property Inspector
        let json = {
            "event": inRegisterEvent,
            "uuid": inPropertyInspectorUUID
        };
        websocket.send(JSON.stringify(json));

        json = {
            "event": "getSettings",
            "context": uuid,
        };
        websocket.send(JSON.stringify(json));

        json = {
            "event": "getGlobalSettings",
            "context": uuid,
        };
        websocket.send(JSON.stringify(json));

		setAction(actionInfo.action);
		updateView();
    };

    websocket.onmessage = function (evt) {
        // Received message from Stream Deck
        const jsonObj = JSON.parse(evt.data);
        if (jsonObj.event === 'didReceiveSettings') {
            const payload = jsonObj.payload.settings;
            console.log(payload);
            document.getElementById('button_address').value = payload.button_address;
            document.getElementById('button_address').value == "undefined" && (document.getElementById('button_address').value = "/button_1");
			document.getElementById('button_value').value = payload.button_value;
            document.getElementById('button_value').value == "undefined" && (document.getElementById('button_value').value = "1.0");
			document.getElementById('layerIndex').value = payload.layerIndex;
            document.getElementById('layerIndex').value == "undefined" && (document.getElementById('layerIndex').value = "0");
			document.getElementById('goToTime').value = payload.goToTime;
            document.getElementById('goToTime').value == "undefined" && (document.getElementById('goToTime').value = "0.0");
        }
        if (jsonObj.event === 'didReceiveGlobalSettings') {
            const payload = jsonObj.payload.settings;

            document.getElementById('ip').value = payload.ip;
            document.getElementById('ip').value == "undefined" && (document.getElementById('ip').value = "127.0.0.1");
            document.getElementById('port').value = payload.port;
            document.getElementById('port').value == "undefined" && (document.getElementById('port').value = "9001");

            const el = document.querySelector('.sdpi-wrapper');
            el && el.classList.remove('hidden');
        }
		updateView();
    };

}

function updateView() {
	/*if (document.getElementById('button_momentary').checked == true) {
		document.getElementById('initial_toggle_value').style.visibility='hidden';
	}
	else {
		document.getElementById('initial_toggle_value').style.visibility='visible';
	}*/
}

function setAction(inAction) {
	if (inAction == "com.elgato.notch.switchLayer") {
		document.getElementById('layerSettings').style.visibility='visible';
		document.getElementById('layerSettings').style.height='auto';
	}

	if (inAction == "com.elgato.notch.momentaryButton" || inAction == "com.elgato.notch.toggleButton") {
		document.getElementById('buttonSettings').style.visibility='visible';
		document.getElementById('buttonSettings').style.height='auto';
	}

		if (inAction == "com.elgato.notch.goToTime") {
		document.getElementById('timeSettings').style.visibility='visible';
		document.getElementById('timeSettings').style.height='auto';
	}
}

function updateLocal() {
	updateView();
    if (websocket && (websocket.readyState === 1)) {
        let payload = {};
        payload.button_address = document.getElementById('button_address').value;
		payload.button_value = parseFloat(document.getElementById('button_value').value);
		payload.layerIndex = parseInt(document.getElementById('layerIndex').value);
		payload.goToTime = parseFloat(document.getElementById('goToTime').value);
        const json = {
            "event": "setSettings",
            "context": uuid,
            "payload": payload
        };
        websocket.send(JSON.stringify(json));
        console.log(json)
    }    
}

function updateGlobal() {
	updateView();
    if (websocket && (websocket.readyState === 1)) {
        let payload = {};
        payload.port = document.getElementById('port').value;
        payload.ip = document.getElementById('ip').value;
        const json = {
            "event": "setGlobalSettings",
            "context": uuid,
            "payload": payload
        };
        websocket.send(JSON.stringify(json));
        console.log(json)
    }    
}

function openPage(site) {
    if (websocket && (websocket.readyState === 1)) {
        const json = {
            'event': 'openUrl',
            'payload': {
                'url': 'https://' + site
            }
        };
        websocket.send(JSON.stringify(json));
    }
}