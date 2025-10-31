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