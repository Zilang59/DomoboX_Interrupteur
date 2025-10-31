window.onload = function() {
  RefreshInfo();
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
  ld2410Interval = setInterval(fetchData, 5000);
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
const inputOn = document.getElementById('inputOn');
const inputOff = document.getElementById('inputOff');

// Quand on clique sur l'icône, ouvre l'input couleur
iconOn.addEventListener('click', () => inputOn.click());
iconOff.addEventListener('click', () => inputOff.click());

// Quand on choisit une couleur, met à jour l'icône
inputOn.addEventListener('input', () => {
  iconOn.style.color = inputOn.value;
});
inputOn.addEventListener('change', () => {
  submitColors();
});

inputOff.addEventListener('input', () => {
  iconOff.style.color = inputOff.value;
});
inputOff.addEventListener('change', () => {
  submitColors();
});

function submitColors() {
  const colorOn = document.getElementById("inputOn").value.slice(1);
  const colorOff = document.getElementById("inputOff").value.slice(1);
  fetch("/option?parametre=1&colorOn=" + colorOn + "&colorOff=" + colorOff)
      .then(response => response.json())
      .then(data => {
          document.getElementById("status").innerHTML = "<span class='success'>Couleurs mises à jour avec succès!</span>";
          setTimeout(() => {
              document.getElementById("status").innerHTML = "";
          }, 3000);
      })
      .catch(error => {
          document.getElementById("status").innerHTML = "<span class='error'>Erreur de mise à jour.</span>";
          setTimeout(() => {
              document.getElementById("status").innerHTML = "";
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

let ld2410Interval;
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
