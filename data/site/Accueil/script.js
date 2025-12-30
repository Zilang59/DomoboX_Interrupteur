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
