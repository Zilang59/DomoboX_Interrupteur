window.onload = function () {
  checkNewFirmware();
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
  const deviceName = "%DISPO_BASENAME%";

  fetch("http://novalys.ovh/UpdateArduino/index.php")
      .then(resp => resp.json())
      .then(files => {
        const binFiles = files
          .map(f => f.name)
          .filter(name => name.startsWith(deviceName + "_") && name.endsWith(".bin"));

        const newer = binFiles.find(name => {
          const version = name.match(/_(\d+\.\d+\.\d+)\.bin$/);
          return version && isNewerVersion(version[1], currentVersion);
        });

        if (newer) {
          boutonUpdate.dataset.file = newer;
          boutonUpdate.style.display = "inline-block";
        } else {
          boutonUpdate.style.display = "none";
        }
      })
      .catch(() => boutonUpdate.style.display = "none");
  }

  function isNewerVersion(v1, v2) {
    const a = v1.split('.').map(Number);
    const b = v2.split('.').map(Number);
    for (let i = 0; i < 3; i++) {
      if (a[i] > b[i]) return true;
      if (a[i] < b[i]) return false;
    }
    return false;
  }



  boutonUpdate.addEventListener("click", () => {
    if(!confirm("Confirmer la mise à jour du firmware ?")) return;

    const fileName = boutonUpdate.dataset.file; // le .bin détecté
    boutonUpdate.style.pointerEvents = "none"; // bloque le bouton
    animateIcon(boutonUpdate);

    fetch(`/update_firmware?file=${encodeURIComponent(fileName)}`)
      .then(response => response.json())
      .then(data => {
        console.log("Mise à jour du firmware lancée");
        // après envoi OTA, on commence à checker si le serveur répond
        setTimeout(() => checkServerAndRefresh(window.location.origin, boutonUpdate), 30000); 
        // 30s de délai pour laisser l'OTA démarrer
      })
      .catch(error => {
        console.log("Erreur lors de la mise à jour du firmware");
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