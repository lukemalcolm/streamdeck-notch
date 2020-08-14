let websocket = null,
    uuid = null,
    actionInfo = {};

var keyUpTimer;

document.addEventListener('DOMContentLoaded', function () {
    initPropertyInspector(100);
});

function initPropertyInspector(initDelay) {
	console.log("Initing")
    /** expermimental carousel is not part of the DOM
     * so let the DOM get constructed first and then
     * inject the carousel */
    setTimeout(function () {
		window.addEventListener('beforeunload', function (e) {
			console.log("Doing shutdown saves...")
			e.preventDefault();
			updateGlobal();
			updateLocal();
			// Don't set a returnValue to the event, otherwise Chromium with throw an error.
		});

    }, initDelay || 100);
}

// To instantly reflect the changes made in the UI, we need to listen to keyup events and then fire saves after a second.
function onKeyUpSave(settingType) {
	// When the key is raised, wait a second and then save the relevant settings.
	clearTimeout(keyUpTimer);
	if (settingType == 'local') {
		f = updateLocal;
	}
	else {
		f = updateGlobal;
	}
	keyUpTimer = setTimeout(f, 1000);

}

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
		console.log(jsonObj.event);

        if (jsonObj.event === 'didReceiveSettings') {
            const payload = jsonObj.payload.settings;
            console.log(payload);
            document.getElementById('button_address').value = payload.button_address;
            document.getElementById('button_address').value == "undefined" && (document.getElementById('button_address').value = "/button_1");
			document.getElementById('button_floatValue').value = payload.button_floatValue;
            document.getElementById('button_floatValue').value == "undefined" && (document.getElementById('button_floatValue').value = "1.0");
			document.getElementById('button_textValue').value = payload.button_textValue;
            document.getElementById('button_textValue').value == "undefined" && (document.getElementById('button_textValue').value = "My Text");
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
	if (inAction == "com.elgato.notch.switchlayer") {
		document.getElementById('layerSettings').style.visibility='visible';
		document.getElementById('layerSettings').style.height='auto';
	}

	if (inAction == "com.elgato.notch.momentarybutton" || inAction == "com.elgato.notch.togglebutton") {
		document.getElementById('buttonSettings').style.visibility='visible';
		document.getElementById('buttonSettings').style.height='auto';
		// Show the numeric value input.
		document.getElementById('floatButtonSection').style.visibility='visible';
		document.getElementById('floatButtonSection').style.height='auto';
	}

	if (inAction == "com.elgato.notch.textbutton") {
		document.getElementById('buttonSettings').style.visibility='visible';
		document.getElementById('buttonSettings').style.height='auto';
		// Show the text value input.
		document.getElementById('textButtonSection').style.visibility='visible';
		document.getElementById('textButtonSection').style.height='auto';
	}

	if (inAction == "com.elgato.notch.gototime") {
		document.getElementById('timeSettings').style.visibility='visible';
		document.getElementById('timeSettings').style.height='auto';
	}

}

function updateLocal() {
	updateView();
    if (websocket && (websocket.readyState === 1)) {
        let payload = {};
        payload.button_address = document.getElementById('button_address').value;
		payload.button_floatValue = parseFloat(document.getElementById('button_floatValue').value);
		payload.button_textValue = document.getElementById('button_textValue').value;
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