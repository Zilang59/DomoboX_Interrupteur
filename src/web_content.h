#pragma once

const char SiteWeb_Accueil[] PROGMEM = R"rawliteral(
<!DOCTYPE html>
<html lang="fr">
<head>
    <meta charset="UTF-8">
    <meta name="viewport" content="width=device-width, initial-scale=1.0">
    <title>%SURNOM%</title>
    <link rel="stylesheet" href="https://www.w3schools.com/w3css/4/w3.css">
    <link href="https://fonts.googleapis.com/css2?family=Orbitron:wght@400;700&display=swap" rel="stylesheet">
    <link href="https://fonts.googleapis.com/icon?family=Material+Icons" rel="stylesheet">
    <style>
* {margin: 0; padding: 0; box-sizing: border-box;}

body {
  display: flex;
  height: 100vh;
  font-family: sans-serif;
  background: #f3f3f3;
}

/* --- MENU LATÉRAL --- */
.sidebar {
  background: #222;
  color: #fff;
  width: 250px;
  transition: width 0.3s;
  display: flex;
  flex-direction: column;
  align-items: stretch; /* prend toute la largeur */
  height: 100%;
  overflow: hidden;
  justify-content: flex-start; /* tout en haut */
  padding-bottom: 20px; /* pour laisser un peu d'espace pour le bas */
}

.menu-top {
  display: flex;
  flex-direction: column;
  width: 100%; /* prend toute la largeur */
  margin-top: 0; /* en haut */
}

.menu-bottom-container {
  display: flex;
  flex-direction: column;
  gap: 10px;
  margin-top: auto; /* pousse tout vers le bas */
}


.sidebar.collapsed {
  width: 70px;
}

.logo {
  font-size: 1.8em;
  margin: 20px 0;
  cursor: pointer;
  user-select: none;
  transition: transform 0.3s;
  transform-origin: center;
  display: flex;
  justify-content: center;
  height: 50px; /* fixe pour éviter le déplacement */
}
.logo:hover { transform: scale(1.1); }

.menu-item {
  width: 100%;
  padding: 15px 20px;
  display: flex;
  align-items: center;
  gap: 10px;
  cursor: pointer;
  transition: background 0.2s;
}

.menu-item-bottom {
  width: 100%;
  padding: 2px 20px;
  display: flex;
  align-items: center;
  gap: 10px;
  cursor: pointer;
  transition: background 0.2s;
}
.menu-item:hover,
.menu-item-bottom:hover {
    background: #333;
}


.menu-item a,
.menu-item-bottom a {
  display: flex;
  align-items: center;
  gap: 10px;
  width: 100%;
  color: inherit;
  text-decoration: none;
}

.menu-item a:hover,
.menu-item-bottom a:hover {
    background: #333;
}

.menu-item-bottom {
  margin-top: auto;
}

.menu-item i, .menu-item-bottom i { font-size: 1.2em; }
.menu-item span, .menu-item-bottom span { transition: opacity 0.3s; }

/* cache le texte des menus normaux et bas quand menu rétracté */
.sidebar.collapsed .menu-item span:not(.material-icons),
.sidebar.collapsed .menu-item-bottom span:not(.material-icons) {
  opacity: 0;
  pointer-events: none;
}

/* --- CONTENU PRINCIPAL --- */
.main {
    flex: 1;
    padding: 30px;
    overflow-y: auto;
    transition: margin-left 0.3s;
}

.main h1 {
  text-align: center;
  margin-bottom: 20px;
}

/* --- RESPONSIVE --- */
@media (max-width: 768px) {
    .sidebar {
        width: 70px; /* étroit par défaut */
    }

    .sidebar.expanded {
        width: 250px; /* s’étend quand on clique sur le logo */
    }

    .sidebar .menu-item span:not(.material-icons),
    .sidebar .menu-item-bottom span:not(.material-icons) {
        opacity: 0;
        pointer-events: none;
    }

    .sidebar.expanded .menu-item span:not(.material-icons),
    .sidebar.expanded .menu-item-bottom span:not(.material-icons) {
        opacity: 1;
        pointer-events: auto;
    }
}










.bouton {
  cursor: pointer;
}

.wait-light {
  margin-top: -20px;
  margin-left: 30px;
  color: #000000;
  animation: spin 2s linear infinite;
  font-size: 20px;
  opacity: 0;              /* invisible au départ */
  transition: opacity 0.7s ease;
  transform-origin: center;
  pointer-events: none;
  display:block;
}

.wait-light.visible {
  opacity: 1;
}
.wait-light.error {
  color: red;
}

@keyframes spin {
  from { transform: rotate(0deg); }
  to { transform: rotate(-360deg); }
}



.section {
  border: 1px solid #ccc;
  border-radius: 10px;
  padding: 15px;
  background-color: #f9f9f9;
  display: flex;
  flex-direction: column;
  gap: 10px;
  width: 100%;
  max-width: 400px; /* limite largeur sur tablette/desktop */
  margin: 0 auto;   /* centre sur grand écran */
  margin-bottom: 10px;
}
.section h2 {
    margin: 0;
    font-size: 1.2em;
    text-align: center;
}
.color-picker-container {
  display: flex;       /* utilise flexbox pour aligner horizontalement */
  gap: 5px;            /* espace entre les icônes */
  flex-wrap: nowrap;   /* empêche le retour à la ligne */
  align-items: center; /* aligne verticalement */
  width:100%;
  justify-content: center;
}
.color-picker-icon {
  margin-left : 10px;
  margin-right : 10px;
  cursor: pointer;
}
.sub-icon {
  position:relative;
  margin-top:30px;
  margin-left:-25px;
  color : #858585;
}
.icon-on { color: gold; }
.icon-off { color: rgb(39, 39, 39); }

.icon-small { font-size: 13px; }
.icon-medium { font-size: 24px; }
.icon-large { font-size: 48px; }
input.color-input {
  position: absolute;
  width: 1px;
  height: 1px;
  opacity: 0;
  pointer-events: none;
}

.color-picker {
  display: flex;
  gap: 10px;
  align-items: center;
}

.color-picker canvas {
  border: 1px solid #ccc;
  cursor: crosshair;
}

.color-palette {
  display: flex;
  flex-wrap: wrap;
  gap: 5px;
  margin-bottom: 10px;
}

.color-swatch {
  width: 25px;
  height: 25px;
  border: 1px solid #ccc;
  cursor: pointer;
  border-radius: 3px;
}






.toggle-container {
    margin-top: 0px;
    text-align: center;
}

.toggle {
    display: flex;
    align-items: center;
    justify-content: center;
    gap: 10px;
}

.toggle input[type="checkbox"] {
    appearance: none;
    width: 50px;
    height: 25px;
    background: #ddd;
    border-radius: 20px;
    position: relative;
    cursor: pointer;
    transition: background 0.3s;
}



.toggle input[type="checkbox"]::before {
    content: "";
    position: absolute;
    width: 20px;
    height: 20px;
    background: white;
    border-radius: 50%;
    top: 2.5px;
    left: 3px;
    transition: transform 0.3s;
}

.toggle input[type="checkbox"]:checked::before {
    transform: translateX(25px);
}


.line {
  display: flex;
  justify-content: space-between;
  align-items: center;
  margin: 8px 0;
}

.label {
  color : black;
}

.sublabel {
  font-size: 10px;
  text-align: right;
  margin-left: auto;
  margin-right: 10px;
}

.sublabel.off {
  color: rgb(160, 160, 160);
}

.sublabel.on {
  color: black;
}

.switch {
  position: relative;
  display: inline-block;
  width: 42px;
  height: 24px;
}
.switch input {
  opacity: 0;
  width: 0;
  height: 0;
}
.slider {
  position: absolute;
  cursor: pointer;
  top: 0; left: 0;
  right: 0; bottom: 0;
  background-color: #ccc;
  transition: .3s;
  border-radius: 24px;
}
.slider:before {
  position: absolute;
  content: "";
  height: 18px; width: 18px;
  left: 3px; bottom: 3px;
  background-color: white;
  transition: .3s;
  border-radius: 50%;
}
input:checked + .slider {
  background-color: #4CAF50;
}
input:checked + .slider:before {
  transform: translateX(18px);
}


.spoiler-btn {
  background-color: #eee;
  border: 1px solid #ccc;
  padding: 1px 12px;
  cursor: pointer;
  border-radius: 4px;
  margin-top: 2px;
  width:100%;
  font-size:12px;
}

.spoiler-btn:hover {
  background-color: #ddd;
}

.spoiler-block {
  display: none;
  border: 1px solid #ccc;
  border-radius: 4px;
  padding: 10px;
  margin-top: 5px;
  background-color: #f9f9f9;
  font-size: 0.9em;
  color: #333;
}

.color-picker-modal {
  position: fixed;
  top: 0;
  left: 0;
  width: 100%;
  height: 100%;
  background: rgba(0, 0, 0, 0.5);
  z-index: 1000;
  display: flex;
  justify-content: center;
  align-items: center;
}

.color-picker-content {
  background: white;
  border-radius: 10px;
  padding: 20px;
  max-width: 90%;
  max-height: 90%;
  overflow: auto;
  position: relative;
}

.color-palette {
  display: flex;
  flex-wrap: wrap;
  gap: 5px;
  margin-bottom: 10px;
}

.color-swatch {
  width: 25px;
  height: 25px;
  border: 1px solid #ccc;
  cursor: pointer;
  border-radius: 3px;
}

.color-picker {
  display: flex;
  gap: 10px;
  align-items: center;
  margin-bottom: 10px;
}

.color-picker canvas {
  border: 1px solid #ccc;
  cursor: crosshair;
}

.modal-open {
  overflow: hidden;
}
</style> <!-- Sera remplacé automatique par la page style.css de ce même répertoire -->
</head>
<body>
    <!-- Menu lateral -->
    <div class="sidebar" id="sidebar">
        <div class="logo" id="logo"><span class="material-icons">apps</span></div>

        <!-- menu du haut -->
        <div class="menu-top">
            <div class="menu-item"><a href="/accueil"><span class="material-icons">home</span><span>Accueil</span></a></div>
            <div class="menu-item"><a href="/wifi"><span class="material-icons">wifi</span><span>Connexion</span></a></div>
            <div class="menu-item"><a href="/informations"><span class="material-icons">info</span><span>Informations</span></a></div>
        </div>

        <!-- menu du bas -->
        <div class="menu-bottom-container">
            <div class="menu-item-bottom" style="display:block;%DISPLAY_MENU_ADMIN%"><a href="/parametres.json"><span class="material-icons">data_object</span><span>Parametres.json</span></a></div>
            <div class="menu-item-bottom" style="display:block;%DISPLAY_MENU_ADMIN%"><a href="/wifi.json"><span class="material-icons">data_object</span><span>Wifi.json</span></a></div>
            <div class="menu-item-bottom" style="display:block;%DISPLAY_MENU_ADMIN%"><a href="/restart"><span class="material-icons">restart_alt</span><span>Redémarrer</span></a></div>
            <div class="menu-item-bottom" style="display:block;%DISPLAY_MENU_ADMIN%"><a href="/reset"><span class="material-icons">credit_card_off</span><span>Remise a zero</span></a></div>
        </div>
    </div>

    <div class="main">
        <h1>Paramètrages <br> <input type="text" id="surnomInput" value="%SURNOM%" style="border: none; background: transparent; font-size: inherit; font-family: inherit; color: inherit; cursor: pointer;text-align:center;" onblur="updateSurnom()" onkeydown="if(event.key==='Enter') { updateSurnom(); this.blur(); }"></h1>

        <div class="section">
            <h2>État de la lumière</h2>
            <div style="position: relative; display: inline-block;">
                <center><span id="etat_bouton_lampe" class="material-icons %ETATLAMPE% icon-on icon-large bouton">light</span>
                <span id="wait_light" class="material-icons wait-light">sync</span></center>
            </div>
        </div>

        <div class="section">
            <h2>Gestion des couleurs</h2>
            <div class="color-picker-container">
                <span class="material-icons color-picker-icon color-picker-on icon-large" style="color:rgb(0, 255, 0);color:#%COULEUR_ON%;" id="iconOn">power_settings_new</span>
                <span class="material-icons color-picker-icon color-picker-on icon-small sub-icon">light_mode</span>
                <span class="material-icons color-picker-icon color-picker-off icon-large" style="color:rgb(255, 0, 0);color:#%COULEUR_OFF%;" id="iconOff">power_settings_new</span>
                <span class="material-icons color-picker-icon color-picker-off icon-small sub-icon">brightness_2</span>

                <!-- Color pickers -->
                <div id="colorPickerOn" class="color-picker-modal" style="display:none;">
                    <div class="color-picker-content">
                        <div class="color-palette">
                            <div class="color-swatch" data-color="#FF0000"></div>
                            <div class="color-swatch" data-color="#00FF00"></div>
                            <div class="color-swatch" data-color="#0000FF"></div>
                            <div class="color-swatch" data-color="#FFFF00"></div>
                            <div class="color-swatch" data-color="#FF00FF"></div>
                            <div class="color-swatch" data-color="#00FFFF"></div>
                            <div class="color-swatch" data-color="#FFA500"></div>
                            <div class="color-swatch" data-color="#800080"></div>
                            <div class="color-swatch" data-color="#FFC0CB"></div>
                            <div class="color-swatch" data-color="#A52A2A"></div>
                            <div class="color-swatch" data-color="#808080"></div>
                            <div class="color-swatch" data-color="#000000"></div>
                            <div class="color-swatch" data-color="#FFFFFF"></div>
                        </div>
                        <div class="color-picker">
                            <canvas id="colorCanvasOn" width="200" height="200"></canvas>
                            <canvas id="hueCanvasOn" width="20" height="200"></canvas>
                        </div>
                        <button id="okOn">OK</button>
                        <button id="cancelOn">Annuler</button>
                    </div>
                </div>
                <div id="colorPickerOff" class="color-picker-modal" style="display:none;">
                    <div class="color-picker-content">
                        <div class="color-palette">
                            <div class="color-swatch" data-color="#FF0000"></div>
                            <div class="color-swatch" data-color="#00FF00"></div>
                            <div class="color-swatch" data-color="#0000FF"></div>
                            <div class="color-swatch" data-color="#FFFF00"></div>
                            <div class="color-swatch" data-color="#FF00FF"></div>
                            <div class="color-swatch" data-color="#00FFFF"></div>
                            <div class="color-swatch" data-color="#FFA500"></div>
                            <div class="color-swatch" data-color="#800080"></div>
                            <div class="color-swatch" data-color="#FFC0CB"></div>
                            <div class="color-swatch" data-color="#A52A2A"></div>
                            <div class="color-swatch" data-color="#808080"></div>
                            <div class="color-swatch" data-color="#000000"></div>
                            <div class="color-swatch" data-color="#FFFFFF"></div>
                        </div>
                        <div class="color-picker">
                            <canvas id="colorCanvasOff" width="200" height="200"></canvas>
                            <canvas id="hueCanvasOff" width="20" height="200"></canvas>
                        </div>
                        <button id="okOff">OK</button>
                        <button id="cancelOff">Annuler</button>
                    </div>
                </div>
            </div>
            <div id="status"></div>
        </div>

        <div class="section" style="display:block;%DISPLAY_MENU_TACTILE%">
            <h2>Gestion des boutons</h2>
            <div class="toggle-container">
                <div class="toggle">
                    <span>Physique</span>
                    <input type="checkbox" id="toggleSwitch" %MODETACTILE% onchange="Changebouton();">
                    <span>Tactile</span>
                </div>
            </div>
        </div>

        <div style="display:block;%DISPLAY_LD2410%" class="section">
            <h2>Détecteur de présence</h2>
            <div class="line">
                <span class="label">Allumage sur détection</span>
                <label class="switch">
                <input type="checkbox" id="ld2410_on_detect" %LD2410_ON_DETECT% onchange="LD2410_on_detect();">
                <span class="slider round"></span>
                </label>
            </div>

            <div class="line">
                <span class="label">Extinction sans détection</span>
                <label class="switch">
                <input type="checkbox" id="ld2410_off_nodetect" %LD2410_OFF_NODETECT% onchange="ld2410_off_nodetect();">
                <span class="slider round"></span>
                </label>
            </div>

            <div class="line">
                <span class="label">Actif uniquement la nuit</span>
                <span id="night_hours" class="sublabel off %LD2410_NIGHT_ONLY_ONOFF%">de %SUNSET%<br>à %SUNRISE%</span>
                <label class="switch">
                <input type="checkbox" id="ld2410_night_only" %LD2410_NIGHT_ONLY% onchange="ld2410_night_only();">
                <span class="slider round"></span>
                </label>
            </div>

            <div class="line">
                <span class="label">Sensibilité</span>
                <input type="range" id="ld2410_sensitivity" min="0" max="255" value="%LD2410_SENSIBILITE%">
            </div>

            <button id="ld2410_spoiler_btn" class="spoiler-btn" style="display:block;%DISPLAY_MENU_ADMIN%">Afficher infos avancées</button>
            <div id="ld2410_spoiler" class="spoiler-block" style="display:none;">
                <p id="ld2410_info">Chargement...</p>
            </div>
        </div>
    </div>
    
    <script>
window.onload = function() {
  RefreshInfo();
  setInitialSliders();
  drawHueCanvas(document.getElementById('hueCanvasOn'));
  drawHueIndicator(document.getElementById('hueCanvasOn'), hOn);
  drawHueCanvas(document.getElementById('hueCanvasOff'));
  drawHueIndicator(document.getElementById('hueCanvasOff'), hOff);
  drawColorCanvas(document.getElementById('colorCanvasOn'), hOn);
  drawColorIndicator(document.getElementById('colorCanvasOn'), sOn, vOn);
  drawColorCanvas(document.getElementById('colorCanvasOff'), hOff);
  drawColorIndicator(document.getElementById('colorCanvasOff'), sOff, vOff);
};



const sidebar = document.getElementById('sidebar');
const logo = document.getElementById('logo');

let ld2410Interval;
let parameterInterval;

function initSidebar() {
  if (window.innerWidth <= 768) {
    sidebar.classList.remove('collapsed');
    sidebar.classList.remove('expanded'); // étroit par défaut sur petit écran
  } else {
    sidebar.classList.remove('expanded');
    sidebar.classList.remove('collapsed'); // étendu par défaut sur grand écran
  }
}

logo.addEventListener('click', () => {
  if (window.innerWidth <= 768) {
    sidebar.classList.toggle('expanded'); // mobile : élargir/réduire
  } else {
    sidebar.classList.toggle('collapsed'); // desktop : réduire/élargir
  }
});

window.addEventListener('resize', initSidebar);
window.addEventListener('load', initSidebar);

function RefreshInfo() {
  // Fonction pour faire la requête
  const fetchData = () => {
    fetch("/parameter_info")
      .then(res => res.json())
      .then(data => {
        // console.log(JSON.stringify(data));
      })
      .catch(() => {
        console.log("impossible de contacter le serveur");
      });
  };

  // Exécuter immédiatement
  fetchData();

  // Puis toutes les 5 secondes
  parameterInterval = setInterval(fetchData, 5000);
}







etat_bouton_lampe.addEventListener('click', () => {
  const wait_light_icon = document.getElementById('wait_light');
  wait_light_icon.classList.add('visible');  // apparition douce
  const etat_bouton_lampe = document.getElementById("etat_bouton_lampe");

  fetch("/option?parametre=4&switchlight=1")
    .then(response => response.json())
    .then(data => {
        const checkLightState = setInterval(() => {
          fetch("/option?parametre=4&switchlight=0")
            .then(response => response.json())
            .then(statusData => {
              const etatLumiere = statusData.EtatLumiere;
              if ((etat_bouton_lampe.classList.contains('icon-on') && etatLumiere === "0") || (etat_bouton_lampe.classList.contains('icon-off') && etatLumiere === "1")) {
                // L'état a changé, on met à jour l'icône
                if (etatLumiere === "1") {
                  etat_bouton_lampe.classList.remove('icon-off');
                  etat_bouton_lampe.classList.add('icon-on');
                } else {
                  etat_bouton_lampe.classList.remove('icon-on');
                  etat_bouton_lampe.classList.add('icon-off');
                }
                wait_light_icon.classList.remove('visible');  // disparition douce
                clearInterval(checkLightState); // Arrêter la vérification
              }
            });
        }, 1000); // Vérifier toutes les secondes

        // Arrêter la vérification après 5 secondes pour éviter une boucle infinie
        setTimeout(() => {
          clearInterval(checkLightState);
          wait_light_icon.classList.add('error');
        }, 5000)
        setTimeout(() => {
          wait_light_icon.classList.remove('visible');  // disparition douce
        }, 6000);
        setTimeout(() => {
          wait_light_icon.classList.remove('error');  // disparition douce
        }, 6700);
    })
    .catch(error => {
      setTimeout(() => {
        wait_light_icon.classList.add('error');
      }, 1000);
      setTimeout(() => {
        wait_light_icon.classList.remove('visible');
      }, 2000);
      setTimeout(() => {
        wait_light_icon.classList.remove('error');
      }, 2700);
    });
});





const iconOn = document.getElementById('iconOn');
const iconOff = document.getElementById('iconOff');

// HSV values
let hOn = 120, sOn = 100, vOn = 100; // Default green
let hOff = 0, sOff = 100, vOff = 100; // Default red

// Initial values for cancel
let initialHOn, initialSOn, initialVOn;
let initialHOff, initialSOff, initialVOff;

function hsvToRgb(h, s, v) {
  h = h / 360;
  s = s / 100;
  v = v / 100;
  let r, g, b;
  const i = Math.floor(h * 6);
  const f = h * 6 - i;
  const p = v * (1 - s);
  const q = v * (1 - f * s);
  const t = v * (1 - (1 - f) * s);
  switch (i % 6) {
    case 0: r = v, g = t, b = p; break;
    case 1: r = q, g = v, b = p; break;
    case 2: r = p, g = v, b = t; break;
    case 3: r = p, g = q, b = v; break;
    case 4: r = t, g = p, b = v; break;
    case 5: r = v, g = p, b = q; break;
  }
  return { r: Math.round(r * 255), g: Math.round(g * 255), b: Math.round(b * 255) };
}

function hexToHsv(hex) {
  const r = parseInt(hex.substr(1,2),16)/255;
  const g = parseInt(hex.substr(3,2),16)/255;
  const b = parseInt(hex.substr(5,2),16)/255;
  const max = Math.max(r, g, b), min = Math.min(r, g, b);
  const v = max;
  const d = max - min;
  const s = max === 0 ? 0 : d / max;
  let h;
  if (max === min) {
    h = 0;
  } else {
    switch (max) {
      case r: h = (g - b) / d + (g < b ? 6 : 0); break;
      case g: h = (b - r) / d + 2; break;
      case b: h = (r - g) / d + 4; break;
    }
    h /= 6;
  }
  return { h: h * 360, s: s * 100, v: v * 100 };
}

function drawHueCanvas(canvas) {
  const ctx = canvas.getContext('2d');
  const gradient = ctx.createLinearGradient(0, 0, 0, canvas.height);
  gradient.addColorStop(0, 'red');
  gradient.addColorStop(1/6, 'yellow');
  gradient.addColorStop(2/6, 'lime');
  gradient.addColorStop(3/6, 'cyan');
  gradient.addColorStop(4/6, 'blue');
  gradient.addColorStop(5/6, 'magenta');
  gradient.addColorStop(1, 'red');
  ctx.fillStyle = gradient;
  ctx.fillRect(0, 0, canvas.width, canvas.height);
}

function drawColorCanvas(canvas, h) {
  const ctx = canvas.getContext('2d');
  const imageData = ctx.createImageData(canvas.width, canvas.height);
  const data = imageData.data;
  for (let y = 0; y < canvas.height; y++) {
    for (let x = 0; x < canvas.width; x++) {
      const s = (x / canvas.width) * 100;
      const v = (1 - y / canvas.height) * 100;
      const rgb = hsvToRgb(h, s, v);
      const index = (y * canvas.width + x) * 4;
      data[index] = rgb.r;
      data[index + 1] = rgb.g;
      data[index + 2] = rgb.b;
      data[index + 3] = 255;
    }
  }
  ctx.putImageData(imageData, 0, 0);
}

function drawHueIndicator(canvas, h) {
  const ctx = canvas.getContext('2d');
  const y = (h / 360) * canvas.height;
  ctx.strokeStyle = 'white';
  ctx.lineWidth = 2;
  ctx.beginPath();
  ctx.moveTo(0, y);
  ctx.lineTo(canvas.width, y);
  ctx.stroke();
  ctx.strokeStyle = 'black';
  ctx.lineWidth = 1;
  ctx.stroke();
}

function drawColorIndicator(canvas, s, v) {
  const ctx = canvas.getContext('2d');
  const x = (s / 100) * canvas.width;
  const y = (1 - v / 100) * canvas.height;
  ctx.strokeStyle = 'white';
  ctx.lineWidth = 2;
  ctx.beginPath();
  ctx.arc(x, y, 5, 0, 2 * Math.PI);
  ctx.stroke();
  ctx.strokeStyle = 'black';
  ctx.lineWidth = 1;
  ctx.stroke();
}

function updateColor(suffix) {
  const h = suffix === 'On' ? hOn : hOff;
  const s = suffix === 'On' ? sOn : sOff;
  const v = suffix === 'On' ? vOn : vOff;
  const rgb = hsvToRgb(h, s, v);
  const color = `rgb(${rgb.r}, ${rgb.g}, ${rgb.b})`;
  const icon = suffix === 'On' ? iconOn : iconOff;
  icon.style.color = color;
}

function setInitialSliders() {
  const colorOn = iconOn.style.color;
  const rgbOn = colorOn.match(/\d+/g);
  if (rgbOn) {
    const r = parseInt(rgbOn[0]);
    const g = parseInt(rgbOn[1]);
    const b = parseInt(rgbOn[2]);
    const hsv = rgbToHsv(r, g, b);
    hOn = hsv.h;
    sOn = hsv.s;
    vOn = hsv.v;
  }
  const colorOff = iconOff.style.color;
  const rgbOff = colorOff.match(/\d+/g);
  if (rgbOff) {
    const r = parseInt(rgbOff[0]);
    const g = parseInt(rgbOff[1]);
    const b = parseInt(rgbOff[2]);
    const hsv = rgbToHsv(r, g, b);
    hOff = hsv.h;
    sOff = hsv.s;
    vOff = hsv.v;
  }
}

function rgbToHsv(r, g, b) {
  r /= 255;
  g /= 255;
  b /= 255;
  const max = Math.max(r, g, b), min = Math.min(r, g, b);
  const v = max;
  const d = max - min;
  const s = max === 0 ? 0 : d / max;
  let h;
  if (max === min) {
    h = 0;
  } else {
    switch (max) {
      case r: h = (g - b) / d + (g < b ? 6 : 0); break;
      case g: h = (b - r) / d + 2; break;
      case b: h = (r - g) / d + 4; break;
    }
    h /= 6;
  }
  return { h: h * 360, s: s * 100, v: v * 100 };
}
function setColorFromHex(suffix, hex) {
  const hsv = hexToHsv(hex);
  if (suffix === 'On') {
    hOn = hsv.h;
    sOn = hsv.s;
    vOn = hsv.v;
    drawColorCanvas(document.getElementById('colorCanvasOn'), hOn);
    drawColorIndicator(document.getElementById('colorCanvasOn'), sOn, vOn);
    drawHueCanvas(document.getElementById('hueCanvasOn'));
    drawHueIndicator(document.getElementById('hueCanvasOn'), hOn);
  } else {
    hOff = hsv.h;
    sOff = hsv.s;
    vOff = hsv.v;
    drawColorCanvas(document.getElementById('colorCanvasOff'), hOff);
    drawColorIndicator(document.getElementById('colorCanvasOff'), sOff, vOff);
    drawHueCanvas(document.getElementById('hueCanvasOff'));
    drawHueIndicator(document.getElementById('hueCanvasOff'), hOff);
  }
  updateColor(suffix);
}

// Event listeners for color swatches
document.querySelectorAll('#colorPickerOn .color-swatch').forEach(swatch => {
  swatch.style.backgroundColor = swatch.dataset.color;
  swatch.addEventListener('click', () => {
    const color = swatch.dataset.color;
    setColorFromHex('On', color);
  });
});

document.querySelectorAll('#colorPickerOff .color-swatch').forEach(swatch => {
  swatch.style.backgroundColor = swatch.dataset.color;
  swatch.addEventListener('click', () => {
    const color = swatch.dataset.color;
    setColorFromHex('Off', color);
  });
});

// Event listeners for canvases
let draggingHue = false, draggingColor = false, currentSuffix = '';

function updateFromMouse(e, canvas, suffix) {
  const rect = canvas.getBoundingClientRect();
  const x = Math.max(0, Math.min(canvas.width, e.clientX - rect.left));
  const y = Math.max(0, Math.min(canvas.height, e.clientY - rect.top));
  if (canvas.id.includes('hue')) {
    const h = (y / canvas.height) * 360;
    if (suffix === 'On') {
      hOn = h;
      drawColorCanvas(document.getElementById('colorCanvasOn'), hOn);
      drawHueCanvas(canvas);
      drawHueIndicator(canvas, hOn);
    } else {
      hOff = h;
      drawColorCanvas(document.getElementById('colorCanvasOff'), hOff);
      drawHueCanvas(canvas);
      drawHueIndicator(canvas, hOff);
    }
  } else {
    const s = (x / canvas.width) * 100;
    const v = (1 - y / canvas.height) * 100;
    if (suffix === 'On') {
      sOn = s;
      vOn = v;
      drawColorCanvas(canvas, hOn);
      drawColorIndicator(canvas, sOn, vOn);
    } else {
      sOff = s;
      vOff = v;
      drawColorCanvas(canvas, hOff);
      drawColorIndicator(canvas, sOff, vOff);
    }
  }
  updateColor(suffix);
}

document.getElementById('hueCanvasOn').addEventListener('mousedown', (e) => { draggingHue = true; currentSuffix = 'On'; updateFromMouse(e, e.target, 'On'); });
document.getElementById('colorCanvasOn').addEventListener('mousedown', (e) => { draggingColor = true; currentSuffix = 'On'; updateFromMouse(e, e.target, 'On'); });
document.getElementById('hueCanvasOff').addEventListener('mousedown', (e) => { draggingHue = true; currentSuffix = 'Off'; updateFromMouse(e, e.target, 'Off'); });
document.getElementById('colorCanvasOff').addEventListener('mousedown', (e) => { draggingColor = true; currentSuffix = 'Off'; updateFromMouse(e, e.target, 'Off'); });

document.addEventListener('mousemove', (e) => {
  if (draggingHue) {
    const canvas = document.getElementById('hueCanvas' + currentSuffix);
    updateFromMouse(e, canvas, currentSuffix);
  } else if (draggingColor) {
    const canvas = document.getElementById('colorCanvas' + currentSuffix);
    updateFromMouse(e, canvas, currentSuffix);
  }
});

document.addEventListener('mouseup', () => { draggingHue = false; draggingColor = false; });

// Quand on clique sur l'icône, ouvre le picker
iconOn.addEventListener('click', () => {
  // Save initial values
  initialHOn = hOn;
  initialSOn = sOn;
  initialVOn = vOn;
  // Show modal
  document.getElementById('colorPickerOn').style.display = 'flex';
  document.body.classList.add('modal-open');
});

iconOff.addEventListener('click', () => {
  // Save initial values
  initialHOff = hOff;
  initialSOff = sOff;
  initialVOff = vOff;
  // Show modal
  document.getElementById('colorPickerOff').style.display = 'flex';
  document.body.classList.add('modal-open');
});

// OK buttons
document.getElementById('okOn').addEventListener('click', () => {
  document.getElementById('colorPickerOn').style.display = 'none';
  document.body.classList.remove('modal-open');
  submitColors();
});

document.getElementById('okOff').addEventListener('click', () => {
  document.getElementById('colorPickerOff').style.display = 'none';
  document.body.classList.remove('modal-open');
  submitColors();
});

// Cancel buttons
document.getElementById('cancelOn').addEventListener('click', () => {
  hOn = initialHOn;
  sOn = initialSOn;
  vOn = initialVOn;
  updateColor('On');
  document.getElementById('colorPickerOn').style.display = 'none';
  document.body.classList.remove('modal-open');
});

document.getElementById('cancelOff').addEventListener('click', () => {
  hOff = initialHOff;
  sOff = initialSOff;
  vOff = initialVOff;
  updateColor('Off');
  document.getElementById('colorPickerOff').style.display = 'none';
  document.body.classList.remove('modal-open');
});

// Close modal on click outside
document.getElementById('colorPickerOn').addEventListener('click', (e) => {
  if (e.target.id === 'colorPickerOn') {
    document.getElementById('colorPickerOn').style.display = 'none';
    document.body.classList.remove('modal-open');
    hOn = initialHOn;
    sOn = initialSOn;
    vOn = initialVOn;
    updateColor('On');
  }
});

document.getElementById('colorPickerOff').addEventListener('click', (e) => {
  if (e.target.id === 'colorPickerOff') {
    document.getElementById('colorPickerOff').style.display = 'none';
    document.body.classList.remove('modal-open');
    hOff = initialHOff;
    sOff = initialSOff;
    vOff = initialVOff;
    updateColor('Off');
  }
});

function submitColors() {
  // Convert rgb to hex
  function rgbToHex(rgb) {
    const result = rgb.match(/\d+/g);
    return ((1 << 24) + (parseInt(result[0]) << 16) + (parseInt(result[1]) << 8) + parseInt(result[2])).toString(16).slice(1);
  }
  const colorOn = rgbToHex(iconOn.style.color);
  const colorOff = rgbToHex(iconOff.style.color);
  fetch("/option?parametre=1&colorOn=" + colorOn + "&colorOff=" + colorOff)
      .then(response => response.json())
      .then(data => {
          // Add a status div if not exists
          let status = document.getElementById("status");
          if (!status) {
            status = document.createElement("div");
            status.id = "status";
            document.querySelector('.color-picker-container').appendChild(status);
          }
          status.innerHTML = "<span class='success'>Couleurs mises à jour avec succès!</span>";
          setTimeout(() => {
              status.innerHTML = "";
          }, 3000);
      })
      .catch(error => {
          let status = document.getElementById("status");
          if (!status) {
            status = document.createElement("div");
            status.id = "status";
            document.querySelector('.color-picker-container').appendChild(status);
          }
          status.innerHTML = "<span class='error'>Erreur de mise à jour.</span>";
          setTimeout(() => {
              status.innerHTML = "";
          }, 3000);
      });
}

let toggleSwitch_initialValue = document.getElementById("toggleSwitch").checked;
function Changebouton() {
  const modeTactile = document.getElementById("toggleSwitch").checked ? 1 : 0;
  fetch("/option?parametre=2&modetactile="+ modeTactile)
    .then(response => response.json())
    .then(data => {
        if(data.status !== "success") {
          setTimeout(() => { document.getElementById("toggleSwitch").checked = toggleSwitch_initialValue; }, 100);
        } else {
          toggleSwitch_initialValue = modeTactile;
        }
    })
    .catch(error => {
        setTimeout(() => { document.getElementById("toggleSwitch").checked = toggleSwitch_initialValue; }, 100);
    });
}













function LD2410_on_detect() {
  const LD2410_on_detect = document.getElementById("ld2410_on_detect").checked ? 1 : 0;
  const ld2410_off_nodetect = document.getElementById("ld2410_off_nodetect").checked ? 1 : 0;
  fetch("/option?parametre=5&LD2410_on_detect="+ LD2410_on_detect)
    .then(response => response.json())
    .then(data => {
        if(data.status === "success") {
          
        }
    })
    .catch(error => {
      setTimeout(() => { document.getElementById("ld2410_on_detect").checked = false; }, 100);
    });
}
function ld2410_off_nodetect() {
  const LD2410_on_detect = document.getElementById("ld2410_on_detect").checked ? 1 : 0;
  const ld2410_off_nodetect = document.getElementById("ld2410_off_nodetect").checked ? 1 : 0;
  fetch("/option?parametre=5&ld2410_off_nodetect="+ ld2410_off_nodetect)
    .then(response => response.json())
    .then(data => {
        if(data.status === "success") {
          
        }
    })
    .catch(error => {
      setTimeout(() => { document.getElementById("ld2410_off_nodetect").checked = false; }, 100);
    });
}
function ld2410_night_only() {
  const LD2410_on_detect = document.getElementById("ld2410_on_detect").checked ? 1 : 0;
  const ld2410_off_nodetect = document.getElementById("ld2410_off_nodetect").checked ? 1 : 0;
  const ld2410_night_only = document.getElementById("ld2410_night_only").checked ? 1 : 0;
  fetch("/option?parametre=5&ld2410_night_only="+ ld2410_night_only)
    .then(response => response.json())
    .then(data => {
        if(data.status === "success" && (LD2410_on_detect === 1 || ld2410_off_nodetect === 1)) {
            setTimeout(() => {
              if(ld2410_night_only === 0){
                document.getElementById("night_hours").classList.remove('on');
                document.getElementById("night_hours").classList.add('off');
              } else {
                document.getElementById("night_hours").classList.remove('off');
                document.getElementById("night_hours").classList.add('on');
              }
            }, 100);
        } else {
          setTimeout(() => { document.getElementById("ld2410_night_only").checked = !ld2410_night_only; }, 100);
        }
    })
    .catch(error => {
      setTimeout(() => { document.getElementById("ld2410_night_only").checked = !ld2410_night_only; }, 100);
    });
}

const sens = document.getElementById("ld2410_sensitivity");
let debounceTimer;
let initialValue = sens.value; // valeur initiale
sens.addEventListener("input", () => {
  clearTimeout(debounceTimer);
  debounceTimer = setTimeout(() => {
    const newValue = sens.value;
    console.log("Envoi au serveur :", newValue);

    fetch("/option?parametre=5&ld2410_sensi=" + newValue)
      .then(response => response.json())
      .then(data => {
        if (data.status !== "success") {
          sens.value = initialValue; // revert si pas succès
        } else {
          initialValue = newValue; // mise à jour de la valeur initiale
        }
      })
      .catch(error => {
        sens.value = initialValue; // revert en cas d'erreur réseau
      });
  }, 500);
});

document.getElementById("ld2410_spoiler_btn").addEventListener("click", () => {
  const spoiler = document.getElementById("ld2410_spoiler");

  if (spoiler.style.display === "none") {
    spoiler.style.display = "block";

    ld2410Interval = setInterval(() => {
      fetch("/ld2410_info")
        .then(res => res.json())
        .then(data => {
          document.getElementById("ld2410_info").textContent = JSON.stringify(data);
        })
        .catch(() => {
          document.getElementById("ld2410_info").textContent = "Erreur de communication";
        });
    }, 1000);

  } else {
    spoiler.style.display = "none";
    clearInterval(ld2410Interval);
  }
});

function updateSurnom() {
  const surnomInput = document.getElementById("surnomInput");
  let newsurnom = surnomInput.value
  fetch("/option?parametre=6&surnom="+ newsurnom)
    .then(response => response.json())
    .then(data => {
        if(data.status === "success") {
          
        }
    })
    .catch(error => {
      setTimeout(() => { document.getElementById("ld2410_on_detect").checked = false; }, 100);
    });
}

</script> <!-- Sera remplacé automatique par la page script.js de ce même répertoire -->
</body>
</html>
)rawliteral";

const char SiteWeb_GestionWifi[] PROGMEM = R"rawliteral(
<!DOCTYPE html>
<html lang="fr">
<head>
    <meta charset="UTF-8">
    <meta name="viewport" content="width=device-width, initial-scale=1.0">
    <title>%DISPO_NAME%</title>
    <link rel="stylesheet" href="https://www.w3schools.com/w3css/4/w3.css">
    <link href="https://fonts.googleapis.com/css2?family=Orbitron:wght@400;700&display=swap" rel="stylesheet">
    <link href="https://fonts.googleapis.com/icon?family=Material+Icons" rel="stylesheet">
    <style>
* {margin: 0; padding: 0; box-sizing: border-box;}

body {
  display: flex;
  height: 100vh;
  font-family: sans-serif;
  background: #f3f3f3;
}

/* --- MENU LATÉRAL --- */
.sidebar {
  background: #222;
  color: #fff;
  width: 250px;
  transition: width 0.3s;
  display: flex;
  flex-direction: column;
  align-items: stretch; /* prend toute la largeur */
  height: 100%;
  overflow: hidden;
  justify-content: flex-start; /* tout en haut */
  padding-bottom: 20px; /* pour laisser un peu d'espace pour le bas */
}

.menu-top {
  display: flex;
  flex-direction: column;
  width: 100%; /* prend toute la largeur */
  margin-top: 0; /* en haut */
}

.menu-bottom-container {
  display: flex;
  flex-direction: column;
  gap: 10px;
  margin-top: auto; /* pousse tout vers le bas */
}


.sidebar.collapsed {
  width: 70px;
}

.logo {
  font-size: 1.8em;
  margin: 20px 0;
  cursor: pointer;
  user-select: none;
  transition: transform 0.3s;
  transform-origin: center;
  display: flex;
  justify-content: center;
  height: 50px; /* fixe pour éviter le déplacement */
}
.logo:hover { transform: scale(1.1); }

.menu-item {
  width: 100%;
  padding: 15px 20px;
  display: flex;
  align-items: center;
  gap: 10px;
  cursor: pointer;
  transition: background 0.2s;
}

.menu-item-bottom {
  width: 100%;
  padding: 2px 20px;
  display: flex;
  align-items: center;
  gap: 10px;
  cursor: pointer;
  transition: background 0.2s;
}
.menu-item:hover,
.menu-item-bottom:hover {
    background: #333;
}


.menu-item a,
.menu-item-bottom a {
  display: flex;
  align-items: center;
  gap: 10px;
  width: 100%;
  color: inherit;
  text-decoration: none;
}

.menu-item a:hover,
.menu-item-bottom a:hover {
    background: #333;
}

.menu-item-bottom {
  margin-top: auto;
}

.menu-item i, .menu-item-bottom i { font-size: 1.2em; }
.menu-item span, .menu-item-bottom span { transition: opacity 0.3s; }

/* cache le texte des menus normaux et bas quand menu rétracté */
.sidebar.collapsed .menu-item span:not(.material-icons),
.sidebar.collapsed .menu-item-bottom span:not(.material-icons) {
  opacity: 0;
  pointer-events: none;
}

/* --- CONTENU PRINCIPAL --- */
.main {
    flex: 1;
    padding: 30px;
    overflow-y: auto;
    transition: margin-left 0.3s;
}

.main h1 {
  text-align: center;
  margin-bottom: 20px;
}

/* --- RESPONSIVE --- */
@media (max-width: 768px) {
    .sidebar {
        width: 70px; /* étroit par défaut */
    }

    .sidebar.expanded {
        width: 250px; /* s’étend quand on clique sur le logo */
    }

    .sidebar .menu-item span:not(.material-icons),
    .sidebar .menu-item-bottom span:not(.material-icons) {
        opacity: 0;
        pointer-events: none;
    }

    .sidebar.expanded .menu-item span:not(.material-icons),
    .sidebar.expanded .menu-item-bottom span:not(.material-icons) {
        opacity: 1;
        pointer-events: auto;
    }
}


.icon-on { color: gold; }
.icon-off { color: gray; }

.icon-small { font-size: 16px; }
.icon-medium { font-size: 24px; }
.icon-large { font-size: 48px; }















.wifi-add-form {
    border: 1px solid #ccc;
    border-radius: 10px;
    padding: 15px;
    background-color: #f9f9f9;
    display: flex;
    flex-direction: column;
    gap: 10px;
    width: 100%;
    max-width: 400px; /* limite largeur sur tablette/desktop */
    margin: 0 auto;   /* centre sur grand écran */
}

.wifi-add-form h2 {
    margin: 0;
    font-size: 1.2em;
    text-align: center;
}

.wifi-add-form input {
    padding: 10px;
    font-size: 1em;
    border-radius: 5px;
    border: 1px solid #ccc;
    width: 100%;
    box-sizing: border-box;
}

.wifi-add-form button {
    padding: 10px;
    font-size: 1em;
    border-radius: 5px;
    border: none;
    background-color: #222;
    color: white;
    cursor: pointer;
}

.wifi-add-form button:hover {
    background-color: #444;
}

.password-container {
  position: relative;
  display: flex;
  align-items: center;
}

.password-container input {
  flex: 1;
  padding-right: 35px;
}

.toggle-mdp {
  position: absolute;
  right: 10px;
  cursor: pointer;
  color: #666;
  user-select: none;
  font-size: 20px;
}

.toggle-mdp:hover {
  color: #000;
}

/* Mobile portrait */
@media (max-width: 480px) {
    .wifi-add-form {
        padding: 10px;
        gap: 8px;
    }

    .wifi-add-form input,
    .wifi-add-form button {
        font-size: 1em;
        padding: 8px;
    }
}

















.wifi-list-section {
  margin-top: 25px;
  border: 1px solid #ccc;
  border-radius: 10px;
  background-color: #f9f9f9;
  padding: 15px;
  max-width: 500px;
  margin-left: auto;
  margin-right: auto;
}

.wifi-list-section h2 {
    margin: 0;
    font-size: 1.2em;
    text-align: center;
}

.wifi-list {
  display: flex;
  flex-direction: column;
  gap: 10px;
}

.wifi-card {
  border: 1px solid #ddd;
  border-radius: 8px;
  padding: 10px;
  background: white;
  display: flex;
  flex-direction: column;
  gap: 5px;
}

.wifi-card-header {
  font-weight: bold;
  color: #222;
}

.wifi-card-info {
  font-size: 0.9em;
  color: #555;
}

.wifi-card-buttons {
  display: flex;
  justify-content: space-between;
  margin-top: 8px;
}

.wifi-card-buttons button {
  flex: 1;
  margin: 0 3px;
  padding: 6px;
  border: none;
  border-radius: 5px;
  cursor: pointer;
  font-size: 0.9em;
}

button.connect { background-color: #222; color: #fff; }
button.modify { background-color: #222; color: #fff; }
button.delete { background-color: #222; color: #fff; }

button.connect:hover { background-color: #444; }
button.modify:hover { background-color: #444; }
button.delete:hover { background-color: #444; }

/* Harmonise la largeur avec le formulaire d’ajout */
.wifi-list-section {
  width: 100%;
  max-width: 400px; /* même largeur que le formulaire */
}

/* Ajoute un léger effet au survol pour les cartes */
.wifi-card:hover {
  background-color: #f3f3f3;
  transition: background 0.2s;
}

.wifi-status-icon {
  float: right;
  font-size: 1.2em;
  vertical-align: middle;
}

.wifi-status-icon.connected { color: rgb(0, 255, 0); }
.wifi-status-icon.disconnected { color: rgb(235, 235, 235); }
.wifi-status-icon.error { color: rgb(255, 0, 0); }

/* Améliore la lisibilité sur mobile */
@media (max-width: 480px) {
  .wifi-card-buttons {
    flex-direction: column;
    gap: 5px;
  }

  .wifi-card-buttons button {
    margin: 0;
    width: 100%;
  }
}
</style> <!-- Sera remplacé automatique par la page style.css de ce même répertoire -->
</head>
<body>
    <!-- Menu lateral -->
    <div class="sidebar" id="sidebar">
        <div class="logo" id="logo"><span class="material-icons">apps</span></div>

        <!-- menu du haut -->
        <div class="menu-top">
            <div class="menu-item"><a href="/accueil"><span class="material-icons">home</span><span>Accueil</span></a></div>
            <div class="menu-item"><a href="/wifi"><span class="material-icons">wifi</span><span>Connexion</span></a></div>
            <div class="menu-item"><a href="/informations"><span class="material-icons">info</span><span>Informations</span></a></div>
        </div>

        <!-- menu du bas -->
        <div class="menu-bottom-container">
            <div class="menu-item-bottom" style="display:block;%DISPLAY_MENU_ADMIN%"><a href="/parametres.json"><span class="material-icons">data_object</span><span>Parametres.json</span></a></div>
            <div class="menu-item-bottom" style="display:block;%DISPLAY_MENU_ADMIN%"><a href="/wifi.json"><span class="material-icons">data_object</span><span>Wifi.json</span></a></div>
            <div class="menu-item-bottom" style="display:block;%DISPLAY_MENU_ADMIN%"><a href="/restart"><span class="material-icons">restart_alt</span><span>Redémarrer</span></a></div>
            <div class="menu-item-bottom" style="display:block;%DISPLAY_MENU_ADMIN%"><a href="/reset"><span class="material-icons">credit_card_off</span><span>Remise a zero</span></a></div>
        </div>
    </div>

    <div class="main">
        <h1>Paramètres Wi-Fi</h1>

        <!-- Formulaire pour ajouter un réseau -->
        <div class="wifi-add-form">
            <h2>Ajouter un réseau</h2>
            <input type="text" id="new-ssid" placeholder="SSID">
            <div class="password-container">
                <input type="password" id="new-mdp" placeholder="Mot de passe">
                <span class="toggle-mdp material-icons">visibility_off</span>
            </div>
            <button id="add-wifi-btn">Ajouter</button>
        </div>

        <!-- Table des réseaux enregistrés -->
        <div class="wifi-list-section">
            <h2>Réseaux enregistrés</h2>
            <div id="wifi-list" class="wifi-list">
                <!-- Les réseaux seront générés ici en JS -->
            </div>
        </div>
    </div>
    
    <script>
const sidebar = document.getElementById('sidebar');
const logo = document.getElementById('logo');

function initSidebar() {
  if (window.innerWidth <= 768) {
    sidebar.classList.remove('collapsed');
    sidebar.classList.remove('expanded'); // étroit par défaut sur petit écran
  } else {
    sidebar.classList.remove('expanded');
    sidebar.classList.remove('collapsed'); // étendu par défaut sur grand écran
  }
}

logo.addEventListener('click', () => {
  if (window.innerWidth <= 768) {
    sidebar.classList.toggle('expanded'); // mobile : élargir/réduire
  } else {
    sidebar.classList.toggle('collapsed'); // desktop : réduire/élargir
  }
});

window.addEventListener('resize', initSidebar);
window.addEventListener('load', initSidebar);



// Ajouter un reseaux un wifi
  const ssidInput = document.getElementById('new-ssid');
  const mdpInput = document.getElementById('new-mdp');
  const addButton = document.getElementById('add-wifi-btn');
  const toggleBtn = document.querySelector('.toggle-mdp');

  toggleBtn.addEventListener('click', () => {
    const isPassword = mdpInput.type === 'password';
    mdpInput.type = isPassword ? 'text' : 'password';
    toggleBtn.textContent = isPassword ? 'visibility' : 'visibility_off';
  });

  addButton.addEventListener('click', () => {
      const newSSID = ssidInput.value.trim();
      const newMDP = mdpInput.value.trim();

      if (newSSID && newMDP) {
          // Envoyer les données au serveur ESP32
          fetch(`/parametres?ssid=${encodeURIComponent(newSSID)}&modifymdp=${encodeURIComponent(newMDP)}`)
              .then(response => response.text())
              .then(data => {
                  console.log('Wifi ajouté :', newSSID);
                  ssidInput.value = '';
                  mdpInput.value = '';
                  loadWifiNetworks(); // recharge la liste après ajout
                  setTimeout(() => {
                      loadWifiNetworks();
                  }, 2000); // attend 2000 ms = 2 secondes
              })
              .catch(error => console.error('Erreur lors de l\'envoi de la commande de modification:', error));
      } else {
          alert('Veuillez remplir les deux champs (SSID et mot de passe).');
      }
  });


// Affichage des resaux wifi enregistrés
  let wifiNetworks = [];
  const ssidConnected = "%SSID_CONNECTED%";
  const ssidRSSI = "%SSID_RSSI%";

  window.onload = function() {
      loadWifiNetworks();
  };

  function loadWifiNetworks() {
      fetch('wifi.json')
      .then(response => response.json())
      .then(data => {
          wifiNetworks = data.wifi_networks;
          displayWifiList();
      });
  }

  function displayWifiList() {
    const section = document.querySelector('.wifi-list-section');
    const wifiListContainer = document.querySelector('.wifi-list');

    wifiListContainer.innerHTML = '';

    const validNetworks = wifiNetworks.filter(net => net.ssid && net.ssid !== "0");

    if (validNetworks.length === 0) {
      section.style.display = 'none';
      return;
    }

    section.style.display = 'block';

    validNetworks.forEach((network, index) => {
      const card = document.createElement('div');
      card.classList.add('wifi-card');

      // Vérifie si ce réseau est celui connecté
      const isConnected = (network.ssid === ssidConnected);
      const IPDisplay = network.IP_local ? network.IP_local + (network.port && network.port !== "80" ? `:${network.port}` : '') : '';
      const DNSDisplay = network.DNS ? network.DNS + (network.port && network.port !== "80" ? `:${network.port}` : '') : '';
      const mDNSDisplay = network.mDNS ? network.mDNS + (network.port && network.port !== "80" ? `:${network.port}` : '') : '';
      
      let logo = "wifi";
      if(ssidRSSI <= -90) { logo = "wifi_1_bar"; }
      else if(ssidRSSI <= -60) { logo = "wifi_2_bar"; }
      else { logo = "wifi"; }

      card.innerHTML = `
        <div class="wifi-card-header">
          ${network.ssid}
          <span class="material-icons wifi-status-icon ${isConnected ? 'connected' : 'disconnected'}">
            ${isConnected ? logo : 'wifi'}
          </span>
        </div>
        <div class="wifi-card-info">
          Nom d'hôte : ${network.hostname || ''}<br>
          Adresse MAC : ${network.MAC_ADRESS || ''}<br>
          Adresse IP : <a href="http://${IPDisplay}" style="text-decoration:none;">${network.IP_local || ''}</a><br>
          DNS (box) : <a href="http://${DNSDisplay}" style="text-decoration:none;">${network.DNS || ''}</a><br>
          mDNS (esp) : <a href="http://${mDNSDisplay}" style="text-decoration:none;">${network.mDNS || ''}</a><br>
          Port : ${network.port || ''}
        </div>
        <div class="wifi-card-buttons">
          <button class="connect">Connexion</button>
          <button class="modify">Modifier</button>
          <button class="delete">Supprimer</button>
        </div>
      `;

      card.querySelector('.connect').onclick = () => ConnectNetwork(index);
      card.querySelector('.modify').onclick = () => modifyPassword(index);
      card.querySelector('.delete').onclick = () => deleteNetwork(index);

      wifiListContainer.appendChild(card);
    });
  }

  function modifyPassword(index) {
    const newPassword = prompt("Entrez le nouveau mot de passe pour le réseau " + wifiNetworks[index].ssid);
    if (newPassword) {
        // Mettre à jour le mot de passe dans le tableau
        fetch(`/parametres?ssid=${wifiNetworks[index].ssid}&modifymdp=${newPassword}`) // Envoie du reseau via une requête HTTP GET
          .then(response => response.text())
          .then(data => console.log('Wifi modifié :', wifiNetworks[index].ssid))
          .catch(error => console.error('Erreur lors de l\'envoi de la commande de modification:', error));

        loadWifiNetworks();
    }
  }
  function deleteNetwork(index) {
    if (confirm("Êtes-vous sûr de vouloir supprimer ce réseau?")) {
        fetch(`/parametres?ssid=${wifiNetworks[index].ssid}&delete=1`) // Envoie la luminosité via une requête HTTP GET
          .then(response => response.text())
          .then(data => console.log('Wifi supprimé :', wifiNetworks[index].ssid))
          .catch(error => console.error('Erreur lors de l\'envoi de la commande de suppression:', error));

        loadWifiNetworks();
    }
  }
  function ConnectNetwork(index) {
      location.reload();
  }
</script> <!-- Sera remplacé automatique par la page script.js de ce même répertoire -->
</body>
</html>
)rawliteral";

const char SiteWeb_Informations[] PROGMEM = R"rawliteral(
<!DOCTYPE html>
<html lang="fr">
<head>
    <meta charset="UTF-8">
    <meta name="viewport" content="width=device-width, initial-scale=1.0">
    <title>%DISPO_NAME%</title>
    <link rel="stylesheet" href="https://www.w3schools.com/w3css/4/w3.css">
    <link href="https://fonts.googleapis.com/css2?family=Orbitron:wght@400;700&display=swap" rel="stylesheet">
    <link href="https://fonts.googleapis.com/icon?family=Material+Icons" rel="stylesheet">
    <style>
* {margin: 0; padding: 0; box-sizing: border-box;}

body {
  display: flex;
  height: 100vh;
  font-family: sans-serif;
  background: #f3f3f3;
}

/* --- MENU LATÉRAL --- */
.sidebar {
  background: #222;
  color: #fff;
  width: 250px;
  transition: width 0.3s;
  display: flex;
  flex-direction: column;
  align-items: stretch; /* prend toute la largeur */
  height: 100%;
  overflow: hidden;
  justify-content: flex-start; /* tout en haut */
  padding-bottom: 20px; /* pour laisser un peu d'espace pour le bas */
}

.menu-top {
  display: flex;
  flex-direction: column;
  width: 100%; /* prend toute la largeur */
  margin-top: 0; /* en haut */
}

.menu-bottom-container {
  display: flex;
  flex-direction: column;
  gap: 10px;
  margin-top: auto; /* pousse tout vers le bas */
}


.sidebar.collapsed {
  width: 70px;
}

.logo {
  font-size: 1.8em;
  margin: 20px 0;
  cursor: pointer;
  user-select: none;
  transition: transform 0.3s;
  transform-origin: center;
  display: flex;
  justify-content: center;
  height: 50px; /* fixe pour éviter le déplacement */
}
.logo:hover { transform: scale(1.1); }

.menu-item {
  width: 100%;
  padding: 15px 20px;
  display: flex;
  align-items: center;
  gap: 10px;
  cursor: pointer;
  transition: background 0.2s;
}

.menu-item-bottom {
  width: 100%;
  padding: 2px 20px;
  display: flex;
  align-items: center;
  gap: 10px;
  cursor: pointer;
  transition: background 0.2s;
}
.menu-item:hover,
.menu-item-bottom:hover {
    background: #333;
}


.menu-item a,
.menu-item-bottom a {
  display: flex;
  align-items: center;
  gap: 10px;
  width: 100%;
  color: inherit;
  text-decoration: none;
}

.menu-item a:hover,
.menu-item-bottom a:hover {
    background: #333;
}

.menu-item-bottom {
  margin-top: auto;
}

.menu-item i, .menu-item-bottom i { font-size: 1.2em; }
.menu-item span, .menu-item-bottom span { transition: opacity 0.3s; }

/* cache le texte des menus normaux et bas quand menu rétracté */
.sidebar.collapsed .menu-item span:not(.material-icons),
.sidebar.collapsed .menu-item-bottom span:not(.material-icons) {
  opacity: 0;
  pointer-events: none;
}

/* --- CONTENU PRINCIPAL --- */
.main {
    flex: 1;
    padding: 30px;
    overflow-y: auto;
    transition: margin-left 0.3s;
}

.main h1 {
  text-align: center;
  margin-bottom: 20px;
}

/* --- RESPONSIVE --- */
@media (max-width: 768px) {
    .sidebar {
        width: 70px; /* étroit par défaut */
    }

    .sidebar.expanded {
        width: 250px; /* s’étend quand on clique sur le logo */
    }

    .sidebar .menu-item span:not(.material-icons),
    .sidebar .menu-item-bottom span:not(.material-icons) {
        opacity: 0;
        pointer-events: none;
    }

    .sidebar.expanded .menu-item span:not(.material-icons),
    .sidebar.expanded .menu-item-bottom span:not(.material-icons) {
        opacity: 1;
        pointer-events: auto;
    }
}














.section {
  border: 1px solid #ccc;
  border-radius: 10px;
  padding: 15px;
  background-color: #f9f9f9;
  display: flex;
  flex-direction: column;
  gap: 10px;
  width: 100%;
  max-width: 400px; /* limite largeur sur tablette/desktop */
  margin: 0 auto;   /* centre sur grand écran */
  margin-bottom: 10px;
  position: relative;
}
.section h2 {
    margin: 0;
    font-size: 1.2em;
    text-align: center;
}
.user-select-none {
  user-select: none;
}

.icon-small { font-size: 13px; }
.icon-medium { font-size: 24px; }
.icon-large { font-size: 48px; }
.bouton { cursor:pointer; }

.top-right-section {
  position: absolute;
  top: 50%;
  transform: translateY(-50%);
  right: 10px;
  color: #858585;
}

.loading-gradient {
  background: linear-gradient(to top, #4caf50 0%, transparent 0%);
  -webkit-background-clip: text;
  -webkit-text-fill-color: transparent;
  transition: background 0.1s linear;
}


.line {
  display: flex;
  justify-content: space-between;
  font-family: monospace;
}

.label {
  flex: 0 0 auto;
}

.dots {
  flex: 1;
  border-bottom: 1px dotted #000;
  margin: 0 5px;
}

.value {
  flex: 0 0 auto;
}

@keyframes blinkRed {
  0%, 100% { color: inherit; }
  50% { color: red; }
}

.blink-red {
  animation: blinkRed 0.2s 5;
}
</style> <!-- Sera remplacé automatique par la page style.css de ce même répertoire -->
</head>
<body>
    <!-- Menu lateral -->
    <div class="sidebar" id="sidebar">
        <div class="logo" id="logo"><span class="material-icons">apps</span></div>

        <!-- menu du haut -->
        <div class="menu-top">
            <div class="menu-item"><a href="/accueil"><span class="material-icons">home</span><span>Accueil</span></a></div>
            <div class="menu-item"><a href="/wifi"><span class="material-icons">wifi</span><span>Connexion</span></a></div>
            <div class="menu-item"><a href="/informations"><span class="material-icons">info</span><span>Informations</span></a></div>
        </div>

        <!-- menu du bas -->
        <div class="menu-bottom-container">
            <div class="menu-item-bottom" style="display:block;%DISPLAY_MENU_ADMIN%"><a href="/parametres.json"><span class="material-icons">data_object</span><span>Parametres.json</span></a></div>
            <div class="menu-item-bottom" style="display:block;%DISPLAY_MENU_ADMIN%"><a href="/wifi.json"><span class="material-icons">data_object</span><span>Wifi.json</span></a></div>
            <div class="menu-item-bottom" style="display:block;%DISPLAY_MENU_ADMIN%"><a href="/restart"><span class="material-icons">restart_alt</span><span>Redémarrer</span></a></div>
            <div class="menu-item-bottom" style="display:block;%DISPLAY_MENU_ADMIN%"><a href="/reset"><span class="material-icons">credit_card_off</span><span>Remise a zero</span></a></div>
        </div>
    </div>

    <div class="main">
        <h1>Informations</h1>

        <div class="section">
            <h2>Version du Firmware</h2>
            <center><span id="version_num" class="user-select-none">%VERSION_FIRMWARE%</span> <span id="bouton_update" style="display:none;" class="material-icons icon-large top-right-section bouton">security_update</span></center>
        </div>

        <div class="section">
            <h2>Modules Connectés</h2>
                <div class="line"><span class="label">Bouton physique "UP"</span><span class="dots"></span><span id="module_up" class="value user-select-none">%MODULE_UP%</span></div>
                <div class="line"><span class="label">Bouton physique "DOWN"</span><span class="dots"></span><span id="module_down" class="value user-select-none">%MODULE_DOWN%</span></div>
                <div class="line"><span class="label">Bouton Tactile</span><span class="dots"></span><span id="module_tactile" class="value user-select-none">%MODULE_TACTILE%</span></div>
                <div class="line"><span class="label">Détecteur de présence</span><span class="dots"></span><span id="module_ld2410" class="value user-select-none">%MODULE_LD2410%</span></div>
                <div class="line"><span class="label">Nombre de relais</span><span class="dots"></span><span id="nombre_relais" class="value user-select-none">%MODULE_RELAIS%</span></div>
        </div>
    </div>
    
    <script>
window.onload = function () {
  checkNewFirmware();
  
  // Vérifier les mises à jour toutes les 5 minutes
  setInterval(checkNewFirmware, 5 * 60 * 1000);
};

const sidebar = document.getElementById('sidebar');
const logo = document.getElementById('logo');

function initSidebar() {
  if (window.innerWidth <= 768) {
    sidebar.classList.remove('collapsed');
    sidebar.classList.remove('expanded'); // étroit par défaut sur petit écran
  } else {
    sidebar.classList.remove('expanded');
    sidebar.classList.remove('collapsed'); // étendu par défaut sur grand écran
  }
}

logo.addEventListener('click', () => {
  if (window.innerWidth <= 768) {
    sidebar.classList.toggle('expanded'); // mobile : élargir/réduire
  } else {
    sidebar.classList.toggle('collapsed'); // desktop : réduire/élargir
  }
});

window.addEventListener('resize', initSidebar);
window.addEventListener('load', initSidebar);


// Permet de passer en mode administrateur et de changer les parametres
  document.addEventListener("DOMContentLoaded", () => {
    const textElement = document.getElementById("version_num");
    const module_up = document.getElementById("module_up");
    const module_down = document.getElementById("module_down");
    const module_tactile = document.getElementById("module_tactile");
    const module_ld2410 = document.getElementById("module_ld2410");
    const nombre_relais = document.getElementById("nombre_relais");

    let clickCount = 0;
    let lastClickTime = 0;
    const elements = [
      {el: module_up, clicks: 2, url: "/option?parametre=3&module_up=1"},
      {el: module_down, clicks: 2, url: "/option?parametre=3&module_down=1"},
      {el: module_tactile, clicks: 2, url: "/option?parametre=3&module_tactile=1"},
      {el: module_ld2410, clicks: 2, url: "/option?parametre=3&module_ld2410=1"},
      {el: nombre_relais, clicks: 2, url: "/option?parametre=3&nombre_relais=1"},
      {el: textElement, clicks: 5, url: "/option?parametre=3&admin=1"}
    ];

    elements.forEach(item => {
      if (!item.el) return; // sécurité si un élément n'existe pas
      item.el.addEventListener("click", () => {
        const now = Date.now();
        if (now - lastClickTime > 400) clickCount = 0;
        clickCount++;
        lastClickTime = now;

        if (clickCount >= item.clicks) {
          clickCount = 0;
          fetch(item.url)
            .then(res => res.json())
            .then(data => {
              if (data.status === "success") {
                setTimeout(() => location.reload(), 2000);
              } else {
                console.log("Action interdite ou non autorisée");
                item.el.classList.add("blink-red");
                setTimeout(() => item.el.classList.remove("blink-red"), 1800); // 3 x 0.6s
              }
            })
            .catch(() => console.log("Pas de reponse du serveur"));
        }
      });
    });
  });


// Permet de lancer la mise a jour du firmware
  const boutonUpdate = document.getElementById("bouton_update");
  let loadingInterval = null;

    
function checkNewFirmware() {
  const boutonUpdate = document.getElementById("bouton_update");
  const currentVersion = document.getElementById("version_num").textContent.trim();

  console.log("🔍 Vérification des mises à jour en cours...");

  // Utilise le nouveau endpoint local qui vérifie GitHub
  fetch("/check_updates")
      .then(resp => {
        console.log(`📡 Réponse HTTP: ${resp.status} ${resp.statusText}`);
        if (!resp.ok) {
          throw new Error(`HTTP ${resp.status}: ${resp.statusText}`);
        }
        return resp.json();
      })
      .then(data => {
        console.log("✅ Vérification des mises à jour:", data);
        
        if (data.status === "success" && data.update_available) {
          // Une mise à jour est disponible
          boutonUpdate.dataset.file = data.download_url;
          boutonUpdate.dataset.version = data.latest_version;
          boutonUpdate.style.display = "inline-block";
          
          // Met à jour le texte du bouton avec la nouvelle version
          const icon = boutonUpdate.querySelector('.material-icons');
          if (icon) {
            boutonUpdate.title = `Mise à jour vers v${data.latest_version} disponible (actuelle: v${data.current_version})`;
          }
          
          console.log(`🆕 Nouvelle version disponible: ${data.latest_version} (actuelle: ${data.current_version})`);
        } else if (data.status === "success") {
          // Pas de mise à jour disponible
          boutonUpdate.style.display = "none";
          console.log(`✅ Firmware à jour (version ${data.current_version})`);
        } else {
          // Erreur dans la réponse
          console.log("❌ Erreur serveur:", data.message || "Réponse inattendue");
          boutonUpdate.style.display = "none";
        }
      })
      .catch(error => {
        console.log("❌ Erreur lors de la vérification des mises à jour:", error.message);
        boutonUpdate.style.display = "none";
        
        // En cas d'erreur réseau, on peut essayer de fallback sur une vérification locale
        if (error.message.includes('Failed to fetch')) {
          console.log("💡 Vérification réseau échouée - mode hors ligne");
        } else if (error.message.includes('500')) {
          console.log("🔧 Erreur serveur - vérifiez la connexion WiFi de l'ESP32");
        }
      });
}


  boutonUpdate.addEventListener("click", () => {
    const fileName = boutonUpdate.dataset.file; // le .bin détecté
    const version = boutonUpdate.dataset.version; // la version détectée
    
    if(!confirm(`Confirmer la mise à jour vers la version ${version} ?`)) return;

    boutonUpdate.style.pointerEvents = "none"; // bloque le bouton
    animateIcon(boutonUpdate);

    fetch(`/update_firmware?file=${encodeURIComponent(fileName)}`)
      .then(response => response.json())
      .then(data => {
        console.log("Mise à jour du firmware lancée depuis GitHub");
        // après envoi OTA, on commence à checker si le serveur répond
        setTimeout(() => checkServerAndRefresh(window.location.origin, boutonUpdate), 30000); 
        // 30s de délai pour laisser l'OTA démarrer
      })
      .catch(error => {
        console.log("Erreur lors de la mise à jour du firmware:", error);
        boutonUpdate.style.pointerEvents = "auto";
        resetIcon(boutonUpdate);
      });
  });

  function checkServerAndRefresh(url, icon) {
    console.log("verrification si le serveur est de nouveau joignable");
    fetch(url) // méthode GET par défaut
      .then(resp => {
        if(resp.ok){
          console.log("c'est tout bon, restart de la page");
          resetIcon(icon);
          location.reload(); // page rafraîchie
        } else {
          console.log("pas joignable, on reanime l'icone");
          animateIcon(icon);
          setTimeout(() => checkServerAndRefresh(url, icon), 2000);
        }
      })
      .catch(() => {
        console.log("pas joignable, on reanime l'icone");
        animateIcon(icon);
        setTimeout(() => checkServerAndRefresh(url, icon), 2000);
      });
  }


  function animateIcon(icon) {
    let progress = 0;
    const steps = 100;           // nombre d'étapes
    const duration = 30000;      // durée totale en ms
    const interval = duration / steps; // temps entre chaque étape

    loadingInterval = setInterval(() => {
      progress += 1; // incrémente de 1% à chaque étape
      if (progress > 100) progress = 100;
      icon.style.background = `linear-gradient(to top, #4caf50 ${progress}%, transparent ${progress}%)`;
    }, interval);
  }

  function resetIcon(icon) {
    clearInterval(loadingInterval);
    icon.style.background = "";
  }
</script> <!-- Sera remplacé automatique par la page script.js de ce même répertoire -->
</body>
</html>
)rawliteral";

