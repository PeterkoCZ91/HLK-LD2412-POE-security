#ifndef WEB_INTERFACE_H
#define WEB_INTERFACE_H

const char index_html[] PROGMEM = R"rawliteral(
<!DOCTYPE HTML>
<html lang="en">
<head>
  <title>LD2412 Security</title>
  <meta charset="UTF-8">
  <meta name="viewport" content="width=device-width, initial-scale=1">
  <link rel="icon" href="data:,">
  <style>
    :root { --bg: #0a0a0a; --card: #161616; --text: #e0e0e0; --accent: #03dac6; --warn: #cf6679; --sec: #333; }
    * { box-sizing: border-box; }
    body { font-family: 'Segoe UI', sans-serif; background: var(--bg); color: var(--text); margin: 0; padding: 10px; padding-bottom: 50px; }
    h2 { color: var(--accent); margin: 5px 0; font-size: 1.4rem; display: flex; align-items: center; justify-content: center; gap: 10px; }

    .grid { display: grid; grid-template-columns: repeat(auto-fit, minmax(300px, 1fr)); gap: 10px; max-width: 1200px; margin: 0 auto; }
    .card { background: var(--card); padding: 15px; border-radius: 12px; box-shadow: 0 4px 10px rgba(0,0,0,0.5); }

    .stat-row { display: flex; justify-content: space-between; margin-bottom: 8px; font-size: 0.9rem; border-bottom: 1px solid #222; padding-bottom: 4px; }
    .stat-val { font-weight: bold; color: #fff; }

    .gauge { text-align: center; margin-bottom: 15px; }
    .big-val { font-size: 2.5rem; font-weight: bold; line-height: 1; }
    .unit { font-size: 0.8rem; color: #888; }

    /* Sparkline */
    svg.spark { width: 100%; height: 50px; stroke-width: 2; fill: none; margin-top: 5px; }

    /* Icons */
    .icon { width: 16px; height: 16px; display: inline-block; vertical-align: middle; border-radius: 50%; }
    .icon.ok { background: #00ff00; box-shadow: 0 0 5px #00ff00; }
    .icon.warn { background: orange; }
    .icon.err { background: #ff0000; }

    /* Inputs */
    input[type=range] { width: 100%; accent-color: var(--accent); }
    input[type=text], input[type=password], input[type=number], select { background: #222; border: 1px solid #444; color: white; padding: 8px; border-radius: 4px; width: 100%; margin-top:2px; }
    .row-input { display:flex; justify-content:space-between; align-items:center; margin-bottom:5px; gap:10px; }

    button { width: 100%; padding: 10px; border: none; border-radius: 6px; background: #3700b3; color: white; cursor: pointer; margin-top: 5px; }
    button:hover { opacity: 0.9; }
    button.sec { background: var(--sec); }
    button.warn { background: var(--warn); color: black; font-weight: bold; }

    /* Per-gate & Bars */
    .gate-wrapper { display: flex; align-items: center; gap: 5px; margin-bottom: 4px; font-size: 0.75rem; }
    .gate-label { width: 25px; flex-shrink:0; }
    input[type=range].mov-slider { accent-color: #03dac6; }
    input[type=range].stat-slider { accent-color: #bb86fc; }
    .gate-dimmed { opacity: 0.35; }

    .tabs { display: flex; gap: 5px; margin-bottom: 10px; flex-wrap: wrap; }
    .tab { flex: 1; min-width: 80px; padding: 8px; background: #222; text-align: center; cursor: pointer; border-radius: 6px; font-size:0.9rem; }
    .tab.active { background: var(--accent); color: black; font-weight: bold; }

    .hidden { display: none; }

    .section-title { color:#888; font-size:0.8rem; margin:15px 0 5px 0; text-transform:uppercase; border-bottom:1px solid #333; }

    #toast { position: fixed; bottom: 20px; left: 50%; transform: translateX(-50%); background: #333; padding: 10px 20px; border-radius: 20px; opacity: 0; transition: opacity 0.3s; pointer-events: none; }

    /* Mobile Responsive */
    @media (max-width: 480px) {
        .grid { grid-template-columns: 1fr; gap: 8px; }
        body { padding: 5px; padding-bottom: 60px; }
        .big-val { font-size: 2rem; }
        .tabs { flex-wrap: wrap; gap: 4px; }
        .tab { min-width: 60px; font-size: 0.8rem; padding: 10px 6px; flex-grow: 1; }
        button { padding: 12px; min-height: 44px; font-size: 1rem; }
        input[type=range] { height: 30px; }
        .gate-wrapper { flex-wrap: wrap; }
        .row-input { flex-direction: column; align-items: stretch; gap: 2px; }
        .row-input span { margin-bottom: 2px; }
        input[type=text], input[type=number], select { padding: 10px; font-size: 1rem; }
    }
  </style>
  <script>
  // i18n — CZ/EN language support
  const I18N = {
    cs: {
      title: "LD2412 Zabezpečení (PoE)", loading: "NAČÍTÁM...", arm: "STŘEŽIT", disarm: "ZRUŠIT",
      disarmed: "🔓 NESTŘEŽENO", arming: "⏳ AKTIVUJI...", armed: "🔒 STŘEŽENO",
      pending: "⚠️ ČEKÁNÍ", triggered: "🚨 POPLACH",
      distance_unit: "VZDÁLENOST (cm)",
      sensor_health: "Zdraví senzoru", uart_state: "UART Stav", frame_rate: "Snímková frekvence",
      comm_errors: "Chyby komunikace",
      ram: "RAM (Volná/Min)", uptime: "Doba běhu",
      chip_temp: "Teplota čipu",
      eth_link: "ETH Link", eth_ip: "IP adresa", eth_speed: "Rychlost ETH",
      static_ip_title: "STATICKÁ IP KONFIGURACE",
      static_ip_enable: "Použít statickou IP",
      static_ip: "Statická IP adresa",
      static_gw: "Výchozí brána",
      static_mask: "Maska podsítě",
      static_dns: "DNS server",
      save_static_ip: "Uložit IP konfiguraci",
      factory_reset_confirm: "Opravdu provést tovární reset radaru?",
      restart_radar: "Restart radaru", restart_esp: "Restart ESP",
      restart_esp_confirm: "Restartovat ESP?",
      tab_basic: "Základní", tab_security: "Bezpečnost", tab_gates: "Hradla",
      tab_network: "Síť & Cloud", tab_zones: "Zóny", tab_events: "Historie",
      device_name: "Jméno zařízení (mDNS)", hold_time: "Doba držení (ms)",
      move_sens: "Citlivost Pohyb (%)", enable_led: "Povolit LED (Indikace)",
      enable_diag: "Povolit Diagnostiku",
      enable_diag_title: "Zapne detailní data hradel (14 zón) a zrychlí komunikaci",
      calib_btn: "Kalibrovat Šum (60s)",
      antimask_title: "ANTI-MASKING (Sabotáž zakrytím)", antimask_enable: "Povolit alarm při tichu",
      timeout_sec: "Časový limit (sec)",
      antimask_hint: "Pro sklady, chaty, serverovny <b>VYPNĚTE</b> - ticho je tam normální.<br>Pro obývané prostory ZAPNĚTE - detekuje zakrytí sensoru.",
      loiter_title: "LOITERING (Podezřelé postávání)", loiter_enable: "Notifikace při postávání",
      loiter_hint: "Alarm když někdo stojí &lt;2m od sensoru déle než timeout.",
      hb_title: "HEARTBEAT (Pravidelný report)", hb_interval: "Interval (hodiny)",
      hb_hint: "0 = vypnuto, 4 = každé 4 hodiny zpráva \"jsem OK\".",
      pet_title: "IMUNITA ZVÍŘAT", pet_energy: "Min. energie pohybu",
      pet_hint: "Filtruje malé objekty (kočky, psi) s nízkou energií &lt;2m.",
      alarm_delay_title: "ZPOŽDĚNÍ ALARMU",
      entry_delay: "Zpoždění vstupu (sec)", exit_delay: "Zpoždění odchodu (sec)",
      disarm_reminder: "Připomínka \"Stále NESTŘEŽENO\"",
      absence_title: "TIMEOUT NEPŘÍTOMNOSTI", absence_duration: "Unmanned Duration (sec)",
      hold_hint: "Doba, po které radar hlásí \"nepřítomnost\" bez detekce.",
      light_title: "SVĚTELNÝ SENZOR (OUT pin)",
      light_func: "Funkce světla", light_off: "Vypnuto",
      light_night: "Noční režim (pod práh)", light_day: "Denní režim (nad práh)",
      light_thr: "Práh světla (0-255)", light_cur: "Aktuální světlo",
      light_hint: "OUT pin aktivní jen když je světlo pod/nad prahem.<br>Ideální pro noční zabezpečení (režim \"pod práh\").",
      movement: "POHYB", static_: "STATIKA", motion: "POHYB",
      gate_legend_mov: "&#9632; Pohyb = citlivost na pohybující se objekty",
      gate_legend_stat: "&#9632; Statika = citlivost na nehybné objekty",
      gate_legend_hint: "Vyšší = citlivější &middot; <span style='opacity:0.4'>šedá = mimo rozsah min/max</span>",
      set_all: "Nastavit vše:", indoor: "Interiér", outdoor: "Exteriér", pets: "Zvířata",
      save_gates: "Uložit Hradla",
      mqtt_title: "MQTT Broker", mqtt_enable: "Povolit MQTT",
      mqtt_server: "IP serveru", mqtt_port: "Port (1883)", mqtt_user: "Uživatel",
      mqtt_pass: "Heslo", save_mqtt: "Uložit MQTT",
      tg_title: "Telegram Notifikace", tg_enable: "Povolit Bot",
      tg_token: "Token bota", tg_chat: "ID chatu",
      tg_save: "Uložit", tg_test: "Otestovat",
      creds_title: "PŘÍSTUPOVÉ ÚDAJE",
      auth_user: "Uživatelské jméno", auth_pass: "Nové heslo", auth_pass2: "Heslo znovu",
      change_pass: "Změnit heslo",
      zone_map_title: "MAPA VZDÁLENOSTÍ", zone_defs_title: "DEFINICE OBLASTÍ (cm)",
      learn_static: "📡 Naučit statiku",
      add_zone: "+ Přidat Zónu", save_zones: "💾 Uložit Zóny",
      recent_events: "POSLEDNÍ UDÁLOSTI",
      clear_history: "Smazat historii",
      evt_th_time: "Čas", evt_th_type: "Typ", evt_th_msg: "Zpráva", evt_th_dist: "Vzdál.",
      fw_update_title: "Aktualizace FW", upload_fw: "Nahrát Firmware",
      default_pass_warn: "⚠️ Výchozí heslo admin/admin — změňte v sekci Síť &amp; Cloud",
      no_events: "Žádné události", del_history: "Smazat celou historii?",
      noise_calib: "Spustit kalibraci šumu? (60s, během této doby se nepohybujte před senzorem)",
      restart_esp_btn: "Restart ESP",
      conn_lost: "Spojení ztraceno",
      enter_creds: "Vyplňte jméno a heslo", pass_mismatch: "Hesla se neshodují",
      creds_changed: "Přihlašovací údaje změněny. Zařízení se restartuje.",
      save_error: "Chyba při ukládání",
      zone_added: "→ Zóna přidána, nezapomeň uložit!",
      zones_saved: "Zóny uloženy",
      gates_saved: "Hradla uložena", gates_error: "Chyba při ukládání hradel",
      preset_applied: "Předvolba nastavena", preset_error: "Chyba presetu",
      restarting: "Restartování...",
      tg_ok: "Telegram OK!", tg_error: "Chyba", tg_unknown: "Neznámá",
      zone_entry_delay: "⏱ Entry delay", zone_immediate: "🚨 Okamžité",
      zone_ignore: "🔕 Ignorovat", zone_ignore_static: "📡 Ignorovat statiku",
      zone_path_any: "— Libovolná —", zone_path_label: "Vstupní cesta:",
      zone_path_title: "Vyžadovaná předchozí zóna (prázdné = libovolná cesta)",
      zone_from: "Od (cm)", zone_to: "Do (cm)", zone_name_ph: "Název",
      zone_default: "Zóna",
      learn_running: "Probíhá", learn_static_pct: "Statika", learn_top_gate: "Top gate",
      learn_done_zone: "✅ Hotovo — navrhovaná zóna",
      learn_apply: "Použít",
      learn_no_static: "⚠️ Žádná výrazná statika nenalezena",
      coverage: "Pokrytí", resolution: "Rozlišení hradlo",
      min_range: "Min. dosah (hradlo)", max_range: "Max. dosah (hradlo)",
      factory_reset_btn: "Reset MW", realtime_ok: "Realtime OK",
      detected: "DETEKCE", hold: "DRŽÍM", idle: "KLID", tamper_state: "SABOTÁŽ!",
      starting: "Spouštím...",
      saved: "Uloženo", ok_btn: "OK",
      min_chars: "Min. 4 znaky",
      pass_changed: "Heslo změněno",
      comm_error: "Chyba komunikace",
    },
    en: {
      title: "LD2412 Security (PoE)", loading: "LOADING...", arm: "ARM", disarm: "DISARM",
      disarmed: "🔓 DISARMED", arming: "⏳ ARMING...", armed: "🔒 ARMED",
      pending: "⚠️ PENDING", triggered: "🚨 TRIGGERED",
      distance_unit: "DISTANCE (cm)",
      sensor_health: "Sensor Health", uart_state: "UART Status", frame_rate: "Frame Rate",
      comm_errors: "Communication Errors",
      ram: "RAM (Free/Min)", uptime: "Uptime",
      chip_temp: "Chip Temperature",
      eth_link: "ETH Link", eth_ip: "IP Address", eth_speed: "ETH Speed",
      static_ip_title: "STATIC IP CONFIGURATION",
      static_ip_enable: "Use Static IP",
      static_ip: "Static IP Address",
      static_gw: "Default Gateway",
      static_mask: "Subnet Mask",
      static_dns: "DNS Server",
      save_static_ip: "Save IP Configuration",
      factory_reset_confirm: "Really perform radar factory reset?",
      restart_radar: "Restart Radar", restart_esp: "Restart ESP",
      restart_esp_confirm: "Restart ESP?",
      tab_basic: "Basic", tab_security: "Security", tab_gates: "Gates",
      tab_network: "Network & Cloud", tab_zones: "Zones", tab_events: "History",
      device_name: "Device Name (mDNS)", hold_time: "Hold Time (ms)",
      move_sens: "Motion Sensitivity (%)", enable_led: "Enable LED (Indicator)",
      enable_diag: "Enable Diagnostics",
      enable_diag_title: "Enable detailed gate data (14 zones) and faster communication",
      calib_btn: "Calibrate Noise (60s)",
      antimask_title: "ANTI-MASKING (Tamper by Covering)", antimask_enable: "Enable silence alarm",
      timeout_sec: "Timeout (sec)",
      antimask_hint: "For warehouses, cabins, server rooms <b>DISABLE</b> — silence is normal there.<br>For occupied spaces ENABLE — detects sensor covering.",
      loiter_title: "LOITERING (Suspicious Lingering)", loiter_enable: "Loitering notification",
      loiter_hint: "Alert when someone stands &lt;2m from sensor longer than timeout.",
      hb_title: "HEARTBEAT (Periodic Report)", hb_interval: "Interval (hours)",
      hb_hint: "0 = disabled, 4 = every 4 hours \"I'm OK\" message.",
      pet_title: "PET IMMUNITY", pet_energy: "Min. motion energy",
      pet_hint: "Filters small objects (cats, dogs) with low energy &lt;2m.",
      alarm_delay_title: "ALARM DELAY",
      entry_delay: "Entry delay (sec)", exit_delay: "Exit delay (sec)",
      disarm_reminder: "\"Still DISARMED\" reminder",
      absence_title: "ABSENCE TIMEOUT", absence_duration: "Unmanned Duration (sec)",
      hold_hint: "Duration after which the radar reports \"unoccupied\" with no detection.",
      light_title: "LIGHT SENSOR (OUT pin)",
      light_func: "Light Function", light_off: "Disabled",
      light_night: "Night mode (below threshold)", light_day: "Day mode (above threshold)",
      light_thr: "Light Threshold (0-255)", light_cur: "Current Light",
      light_hint: "OUT pin active only when light is below/above threshold.<br>Ideal for night security (below-threshold mode).",
      movement: "MOVEMENT", static_: "STATIC", motion: "MOTION",
      gate_legend_mov: "&#9632; Motion = sensitivity to moving objects",
      gate_legend_stat: "&#9632; Static = sensitivity to stationary objects",
      gate_legend_hint: "Higher = more sensitive &middot; <span style='opacity:0.4'>gray = outside min/max range</span>",
      set_all: "Set all:", indoor: "Indoor", outdoor: "Outdoor", pets: "Pet",
      save_gates: "Save Gates",
      mqtt_title: "MQTT Broker", mqtt_enable: "Enable MQTT",
      mqtt_server: "Server IP", mqtt_port: "Port (1883)", mqtt_user: "Username",
      mqtt_pass: "Password", save_mqtt: "Save MQTT",
      tg_title: "Telegram Notifications", tg_enable: "Enable Bot",
      tg_token: "Bot token", tg_chat: "Chat ID",
      tg_save: "Save", tg_test: "Test",
      creds_title: "CREDENTIALS",
      auth_user: "Username", auth_pass: "New password", auth_pass2: "Confirm password",
      change_pass: "Change Password",
      zone_map_title: "DISTANCE MAP", zone_defs_title: "ZONE DEFINITIONS (cm)",
      learn_static: "📡 Learn Static",
      add_zone: "+ Add Zone", save_zones: "💾 Save Zones",
      recent_events: "RECENT EVENTS",
      clear_history: "Clear History",
      evt_th_time: "Time", evt_th_type: "Type", evt_th_msg: "Message", evt_th_dist: "Dist.",
      fw_update_title: "FW Update", upload_fw: "Upload Firmware",
      default_pass_warn: "⚠️ Default password admin/admin — change in Network &amp; Cloud section",
      no_events: "No events", del_history: "Clear all history?",
      noise_calib: "Start noise calibration? (60s, do not move in front of sensor during this time)",
      restart_esp_btn: "Restart ESP",
      conn_lost: "Connection lost",
      enter_creds: "Enter username and password", pass_mismatch: "Passwords do not match",
      creds_changed: "Credentials changed. Device will restart.",
      save_error: "Save error",
      zone_added: "→ Zone added, don't forget to save!",
      zones_saved: "Zones saved",
      gates_saved: "Gates saved", gates_error: "Gate save error",
      preset_applied: "Preset applied", preset_error: "Preset error",
      restarting: "Restarting...",
      tg_ok: "Telegram OK!", tg_error: "Error", tg_unknown: "Unknown",
      zone_entry_delay: "⏱ Entry delay", zone_immediate: "🚨 Immediate",
      zone_ignore: "🔕 Ignore", zone_ignore_static: "📡 Ignore static",
      zone_path_any: "— Any —", zone_path_label: "Entry path:",
      zone_path_title: "Required previous zone (empty = any path)",
      zone_from: "From (cm)", zone_to: "To (cm)", zone_name_ph: "Name",
      zone_default: "Zone",
      learn_running: "Running", learn_static_pct: "Static", learn_top_gate: "Top gate",
      learn_done_zone: "✅ Done — suggested zone",
      learn_apply: "Apply",
      learn_no_static: "⚠️ No significant static found",
      coverage: "Coverage", resolution: "Resolution gate",
      min_range: "Min Range (Gate)", max_range: "Max Range (Gate)",
      factory_reset_btn: "Reset MW", realtime_ok: "Realtime OK",
      detected: "DETECTED", hold: "HOLDING", idle: "IDLE", tamper_state: "TAMPER!",
      starting: "Starting...",
      saved: "Saved", ok_btn: "OK",
      min_chars: "Min. 4 characters",
      pass_changed: "Password changed",
      comm_error: "Communication error",
    }
  };
  const DEFAULT_LANG = 'en';
  let LANG = localStorage.getItem('lang') || DEFAULT_LANG;
  if (!I18N[LANG]) LANG = DEFAULT_LANG;
  function t(k) { return (I18N[LANG] && I18N[LANG][k]) || (I18N.en[k]) || k; }
  function setLang(l) { LANG = l; localStorage.setItem('lang', l); applyLang(); }
  function applyLang() {
    document.documentElement.lang = LANG;
    document.querySelectorAll('[data-i18n]').forEach(el => {
      let k = el.getAttribute('data-i18n');
      if (el.tagName === 'INPUT') el.placeholder = t(k);
      else if (el.tagName === 'OPTION') el.textContent = t(k);
      else el.innerHTML = t(k);
    });
    document.querySelector('#lang_btn').textContent = LANG === 'cs' ? '🇬🇧 EN' : '🇨🇿 CZ';
    document.title = t('title');
  }
  </script>
</head>
<body>

  <h2>
    LD2412 <span style="font-size:0.6em; color:#666" id="fw_ver">...</span>
    <span id="sse_icon" class="icon" title="Realtime connection"></span>
    <span id="wifi_icon" class="icon" title="ETH"></span>
    <span id="mqtt_icon" class="icon" title="MQTT"></span>
    <button id="lang_btn" onclick="setLang(LANG==='cs'?'en':'cs')" style="width:auto; padding:2px 8px; font-size:0.7rem; background:#333; border-radius:4px; margin:0; min-height:auto">🇨🇿 CZ</button>
  </h2>

  <div id="security_warning" style="background:#cf6679; color:black; padding:10px; border-radius:8px; margin-bottom:10px; display:none; text-align:center; font-weight:bold;">
    <span data-i18n="default_pass_warn">⚠️ Default password admin/admin — change in Network &amp; Cloud section</span>
  </div>

  <div class="grid">
    <!-- MAIN STATUS -->
    <div class="card">
        <div class="gauge">
            <div id="state_text" style="color:#888; font-weight:bold; letter-spacing:2px; margin-bottom:5px" data-i18n="loading">LOADING...</div>
            <div id="alarm_badge" style="margin-bottom:8px; font-size:0.9rem; font-weight:bold; color:#888">---</div>
            <button id="btn_arm" onclick="toggleArm()" style="width:auto; padding:8px 20px; margin-bottom:10px; background:#3700b3" data-i18n="arm">ARM</button>
            <div class="big-val" id="dist_val" style="color:var(--accent)">---</div>
            <div class="unit" data-i18n="distance_unit">DISTANCE (cm)</div>
            <svg class="spark" id="graph_dist"></svg>
        </div>
        <div style="display:flex; gap:10px">
            <div style="flex:1; text-align:center">
                <div style="color:#03dac6; font-weight:bold" id="mov_val">0%</div>
                <div class="unit" data-i18n="motion">MOTION</div>
                <svg class="spark" id="graph_mov" style="height:30px; stroke:#03dac6"></svg>
            </div>
            <div style="flex:1; text-align:center">
                <div style="color:#bb86fc; font-weight:bold" id="stat_val">0%</div>
                <div class="unit" data-i18n="static_">STATIC</div>
                <svg class="spark" id="graph_stat" style="height:30px; stroke:#bb86fc"></svg>
            </div>
        </div>
    </div>

    <!-- HEALTH & STATS -->
    <div class="card">
        <div class="stat-row"><span data-i18n="sensor_health">Sensor Health</span><span id="h_score" class="stat-val">---%</span></div>
        <div class="stat-row"><span data-i18n="uart_state">UART Status</span><span id="h_uart">---</span></div>
        <div class="stat-row"><span data-i18n="frame_rate">Frame Rate</span><span id="h_fps">--- FPS</span></div>
        <div class="stat-row"><span data-i18n="comm_errors">Communication Errors</span><span id="h_err" style="color:var(--warn)">0</span></div>
        <div class="stat-row"><span data-i18n="ram">RAM (Free/Min)</span><span id="h_heap">--- / --- KB</span></div>
        <div class="stat-row"><span data-i18n="chip_temp">Chip Temperature</span><span id="h_temp">--- °C</span></div>
        <div class="stat-row"><span data-i18n="eth_link">ETH Link</span><span id="h_eth_link" class="stat-val">---</span></div>
        <div class="stat-row"><span data-i18n="uptime">Uptime</span><span id="h_uptime">---</span></div>
        <div style="display:flex; gap:5px; margin-top:10px; flex-wrap: wrap;">
            <button class="sec" style="flex:1; min-width:80px;" onclick="api('radar/restart', {method:'POST'})" data-i18n="restart_radar">Restart Radar</button>
            <button class="sec" style="flex:1; min-width:80px;" onclick="if(confirm(t('restart_esp_confirm'))) api('restart', {method:'POST'})" data-i18n="restart_esp">Restart ESP</button>
            <button class="warn" style="flex:1; min-width:80px;" onclick="if(confirm(t('factory_reset_confirm'))) api('radar/factory_reset', {method:'POST'})" data-i18n="factory_reset_btn">Reset MW</button>
        </div>
    </div>

    <!-- CONTROLS -->
    <div class="card">
        <div class="tabs">
            <div class="tab active" onclick="tab(0)" data-i18n="tab_basic">Basic</div>
            <div class="tab" onclick="tab(1)" data-i18n="tab_security">Security</div>
            <div class="tab" onclick="tab(2)" data-i18n="tab_gates">Gates</div>
            <div class="tab" onclick="tab(3)" data-i18n="tab_network">Network &amp; Cloud</div>
            <div class="tab" onclick="tab(4)" data-i18n="tab_zones">Zones</div>
            <div class="tab" onclick="tab(5)" data-i18n="tab_events">History</div>
        </div>

        <!-- TAB 0: BASIC -->
        <div id="tab0">
            <div class="stat-row"><span data-i18n="device_name">Device Name (mDNS)</span></div>
            <div style="display:flex; gap:5px; margin-bottom:10px">
                <input type="text" id="txt_hostname" placeholder="e.g. sensor-livingroom">
                <button class="sec" style="width:auto; margin:0" onclick="saveHostname()" data-i18n="ok_btn">OK</button>
            </div>

            <div class="row-input">
                <span style="flex:1" data-i18n="min_range">Min Range (Gate)</span>
                <input type="number" id="i_min" min="0" max="13" style="width:60px" onchange="saveBasic()">
            </div>
            <div class="row-input">
                <span style="flex:1" data-i18n="max_range">Max Range (Gate)</span>
                <input type="number" id="i_max" min="1" max="13" style="width:60px" onchange="saveBasic()">
            </div>

            <div class="stat-row" style="margin-top:10px"><span data-i18n="hold_time">Hold Time (ms)</span></div>
            <input type="number" id="i_hold" step="1000" onchange="saveBasic()">

            <div class="stat-row" style="margin-top:10px"><span data-i18n="move_sens">Motion Sensitivity (%)</span></div>
            <input type="number" id="i_sens" min="0" max="100" onchange="saveBasic()">

            <div style="display:flex; align-items:center; gap:8px; margin-top:15px; margin-bottom:5px">
                <input type="checkbox" id="chk_led" style="width:auto" onchange="saveBasic()">
                <label for="chk_led" data-i18n="enable_led">Enable LED (Indicator)</label>
            </div>
            <div style="display:flex; align-items:center; gap:8px; margin-bottom:10px">
                <input type="checkbox" id="chk_eng" style="width:auto" onchange="toggleEng()">
                <label for="chk_eng" id="lbl_enable_diag" data-i18n="enable_diag">Enable Diagnostics</label>
            </div>

            <button id="btn_calib" onclick="startCalib()" style="margin-top:15px" data-i18n="calib_btn">Calibrate Noise (60s)</button>
        </div>

        <!-- TAB 1: SECURITY -->
        <div id="tab1" class="hidden">
            <div class="section-title" data-i18n="antimask_title">ANTI-MASKING (Tamper by Covering)</div>
            <div style="display:flex; align-items:center; gap:8px; margin-bottom:5px">
                <input type="checkbox" id="chk_am_en" style="width:auto" onchange="saveSec()">
                <label for="chk_am_en" data-i18n="antimask_enable">Enable silence alarm</label>
            </div>
            <div class="row-input">
                <span data-i18n="timeout_sec">Timeout (sec)</span>
                <input type="number" id="i_am" placeholder="300" style="width:80px" onchange="saveSec()">
            </div>
            <p style="font-size:0.7rem; color:#666; margin:2px 0 15px 0">
                ⚠️ <span data-i18n="antimask_hint">For warehouses, cabins, server rooms <b>DISABLE</b> — silence is normal there.<br>For occupied spaces ENABLE — detects sensor covering.</span>
            </p>

            <div class="section-title" data-i18n="loiter_title">LOITERING (Suspicious Lingering)</div>
            <div style="display:flex; align-items:center; gap:8px; margin-bottom:5px">
                <input type="checkbox" id="chk_loit_en" style="width:auto" onchange="saveSec()">
                <label for="chk_loit_en" data-i18n="loiter_enable">Loitering notification</label>
            </div>
            <div class="row-input">
                <span data-i18n="timeout_sec">Timeout (sec)</span>
                <input type="number" id="i_loit" placeholder="15" style="width:80px" onchange="saveSec()">
            </div>
            <p style="font-size:0.7rem; color:#666; margin:2px 0 15px 0">
                <span data-i18n="loiter_hint">Alert when someone stands &lt;2m from sensor longer than timeout.</span>
            </p>

            <div class="section-title" data-i18n="hb_title">HEARTBEAT (Periodic Report)</div>
            <div class="row-input">
                <span data-i18n="hb_interval">Interval (hours)</span>
                <input type="number" id="i_hb" placeholder="4" min="0" max="24" style="width:80px" onchange="saveSec()">
            </div>
            <p style="font-size:0.7rem; color:#666; margin:2px 0 15px 0">
                <span data-i18n="hb_hint">0 = disabled, 4 = every 4 hours "I'm OK" message.</span>
            </p>

            <div class="section-title" data-i18n="pet_title">PET IMMUNITY</div>
            <div class="row-input">
                <span data-i18n="pet_energy">Min. motion energy</span>
                <input type="number" id="i_pet" placeholder="10" min="0" max="50" style="width:80px" onchange="saveSec()">
            </div>
            <p style="font-size:0.7rem; color:#666; margin:2px 0">
                <span data-i18n="pet_hint">Filters small objects (cats, dogs) with low energy &lt;2m.</span>
            </p>

            <div class="section-title" data-i18n="alarm_delay_title">ALARM DELAY</div>
            <div class="row-input">
                <span data-i18n="entry_delay">Entry delay (sec)</span>
                <input type="number" id="i_entry_dl" placeholder="30" min="0" max="300" style="width:80px" onchange="saveAlarmConfig()">
            </div>
            <div class="row-input">
                <span data-i18n="exit_delay">Exit delay (sec)</span>
                <input type="number" id="i_exit_dl" placeholder="30" min="0" max="300" style="width:80px" onchange="saveAlarmConfig()">
            </div>
            <div style="display:flex; align-items:center; gap:8px; margin-bottom:5px">
                <input type="checkbox" id="chk_dis_rem" style="width:auto" onchange="saveAlarmConfig()">
                <label for="chk_dis_rem" data-i18n="disarm_reminder">"Still DISARMED" reminder</label>
            </div>

            <div class="section-title" data-i18n="absence_title">ABSENCE TIMEOUT</div>
            <div class="row-input">
                <span data-i18n="absence_duration">Unmanned Duration (sec)</span>
                <input type="number" id="i_timeout" placeholder="10" min="0" max="255" style="width:80px" onchange="saveTimeout()">
            </div>
            <p style="font-size:0.7rem; color:#666; margin:2px 0 15px 0">
                <span data-i18n="hold_hint">Duration after which the radar reports "unoccupied" with no detection.</span>
            </p>

            <div class="section-title" data-i18n="light_title">LIGHT SENSOR (OUT pin)</div>
            <div class="row-input">
                <span data-i18n="light_func">Light Function</span>
                <select id="sel_light_func" style="width:140px" onchange="saveLightConfig()">
                    <option value="0" data-i18n="light_off">Disabled</option>
                    <option value="1" data-i18n="light_night">Night mode (below threshold)</option>
                    <option value="2" data-i18n="light_day">Day mode (above threshold)</option>
                </select>
            </div>
            <div class="row-input">
                <span data-i18n="light_thr">Light Threshold (0-255)</span>
                <input type="number" id="i_light_thresh" placeholder="128" min="0" max="255" style="width:80px" onchange="saveLightConfig()">
            </div>
            <div class="row-input">
                <span data-i18n="light_cur">Current Light</span>
                <span id="cur_light_val" style="font-weight:bold">---</span>
            </div>
            <p style="font-size:0.7rem; color:#666; margin:2px 0 0 0">
                <span data-i18n="light_hint">OUT pin active only when light is below/above threshold.<br>Ideal for night security (below-threshold mode).</span>
            </p>
        </div>

        <!-- TAB 2: GATES -->
        <div id="tab2" class="hidden">
            <div id="range_summary" style="font-size:0.8rem; color:#888; margin-bottom:8px; text-align:center"></div>

            <div style="font-size:0.75rem; color:#888; margin-bottom:8px; line-height:1.5">
                <span style="color:#03dac6; font-weight:bold" data-i18n="gate_legend_mov">&#9632; Motion = sensitivity to moving objects</span> &middot;
                <span style="color:#bb86fc; font-weight:bold" data-i18n="gate_legend_stat">&#9632; Static = sensitivity to stationary objects</span><br>
                <span data-i18n="gate_legend_hint">Higher = more sensitive &middot; <span style='opacity:0.4'>gray = outside min/max range</span></span>
            </div>

            <div style="background:#111; border-radius:8px; padding:8px; margin-bottom:8px">
                <div style="font-size:0.75rem; color:#888; margin-bottom:4px" data-i18n="set_all">Set all:</div>
                <div style="display:flex; gap:6px; align-items:center; flex-wrap:wrap">
                    <span style="color:#03dac6; font-size:0.75rem; width:12px">P</span>
                    <input type="range" class="mov-slider" id="g_m_all" value="50" min="0" max="100" style="flex:1; min-width:60px" oninput="$('lm_all').innerText=this.value">
                    <span id="lm_all" style="width:22px; color:#03dac6; font-size:0.75rem; text-align:right">50</span>
                    <span style="color:#bb86fc; font-size:0.75rem; width:12px; margin-left:4px">S</span>
                    <input type="range" class="stat-slider" id="g_s_all" value="30" min="0" max="100" style="flex:1; min-width:60px" oninput="$('ls_all').innerText=this.value">
                    <span id="ls_all" style="width:22px; color:#bb86fc; font-size:0.75rem; text-align:right">30</span>
                    <button class="sec" style="width:auto; padding:4px 10px; margin:0; font-size:0.75rem" onclick="setAllGates()">OK</button>
                </div>
            </div>

            <div style="display:flex; justify-content:space-between; margin-bottom:8px; gap:5px">
                <button class="sec" style="flex:1; padding:5px; font-size:0.8rem" onclick="setPreset('indoor')" data-i18n="indoor">Indoor</button>
                <button class="sec" style="flex:1; padding:5px; font-size:0.8rem" onclick="setPreset('outdoor')" data-i18n="outdoor">Outdoor</button>
                <button class="sec" style="flex:1; padding:5px; font-size:0.8rem" onclick="setPreset('pet')" data-i18n="pets">Pet</button>
            </div>

            <div id="gates_container" style="max-height:400px; overflow-y:auto"></div>

            <button onclick="saveGates()" style="margin-top:8px" data-i18n="save_gates">Save Gates</button>
        </div>

        <!-- TAB 3: NETWORK & CLOUD -->
        <div id="tab3" class="hidden">
            <div class="section-title" data-i18n="mqtt_title">MQTT Broker</div>
            <div style="display:flex; align-items:center; gap:8px;">
                <input type="checkbox" id="chk_mqtt_en" style="width:auto">
                <label for="chk_mqtt_en" data-i18n="mqtt_enable">Enable MQTT</label>
            </div>
            <input type="text" id="txt_mqtt_server" data-i18n="mqtt_server" placeholder="Server IP">
            <div style="display:flex; gap:5px">
                <input type="text" id="txt_mqtt_port" placeholder="Port (1883)">
                <input type="text" id="txt_mqtt_user" data-i18n="mqtt_user" placeholder="Username">
            </div>
            <input type="password" id="txt_mqtt_pass" data-i18n="mqtt_pass" placeholder="Password">
            <button onclick="saveMQTTConfig()" class="sec" data-i18n="save_mqtt">Save MQTT</button>

            <div class="section-title" data-i18n="static_ip_title">STATIC IP CONFIGURATION</div>
            <div style="display:flex; align-items:center; gap:8px; margin-bottom:5px">
                <input type="checkbox" id="chk_static_ip" style="width:auto">
                <label for="chk_static_ip" data-i18n="static_ip_enable">Use Static IP</label>
            </div>
            <input type="text" id="txt_static_ip" data-i18n="static_ip" placeholder="Static IP Address">
            <input type="text" id="txt_static_gw" data-i18n="static_gw" placeholder="Default Gateway">
            <input type="text" id="txt_static_mask" data-i18n="static_mask" placeholder="Subnet Mask">
            <input type="text" id="txt_static_dns" data-i18n="static_dns" placeholder="DNS Server">
            <button onclick="saveStaticIP()" class="sec" data-i18n="save_static_ip">Save IP Configuration</button>

            <div class="section-title" data-i18n="tg_title">Telegram Notifications</div>
            <div style="display:flex; align-items:center; gap:8px;">
                <input type="checkbox" id="chk_tg_en" style="width:auto">
                <label for="chk_tg_en" data-i18n="tg_enable">Enable Bot</label>
            </div>
            <input type="text" id="txt_tg_token" data-i18n="tg_token" placeholder="Bot token">
            <input type="text" id="txt_tg_chat" data-i18n="tg_chat" placeholder="Chat ID">
            <div style="display:flex; gap:5px">
                <button onclick="saveTelegram()" class="sec" data-i18n="tg_save">Save</button>
                <button onclick="testTelegram()" class="sec" data-i18n="tg_test">Test</button>
            </div>
            <div class="section-title" data-i18n="creds_title">CREDENTIALS</div>
            <input type="text" id="txt_auth_user" data-i18n="auth_user" placeholder="Username">
            <input type="password" id="txt_auth_pass" data-i18n="auth_pass" placeholder="New password">
            <input type="password" id="txt_auth_pass2" data-i18n="auth_pass2" placeholder="Confirm password">
            <button onclick="saveAuth()" class="warn" data-i18n="change_pass">Change Password</button>
        </div>

        <!-- TAB 4: ZONES -->
        <div id="tab4" class="hidden">
            <div class="label" style="font-size:0.8rem; color:#888; margin-bottom:4px" data-i18n="zone_map_title">DISTANCE MAP</div>
            <div style="position:relative; margin-bottom:10px">
                <svg id="zone_map" width="100%" height="48" style="display:block"></svg>
                <div style="position:absolute; bottom:2px; left:4px; font-size:0.65rem; color:#555" id="zone_map_scale"></div>
            </div>
            <div class="label" style="margin-bottom:10px; font-size:0.8rem; color:#888" data-i18n="zone_defs_title">ZONE DEFINITIONS (cm)</div>
            <div id="zones_list"></div>
            <button onclick="addZone()" class="sec" style="margin-top:10px" data-i18n="add_zone">+ Add Zone</button>
            <button onclick="saveZones()" data-i18n="save_zones">💾 Save Zones</button>
            <!-- Auto-learn -->
            <div style="background:#1a1a2e; border-radius:6px; padding:8px; margin-top:10px">
                <div style="display:flex; gap:6px; align-items:center; flex-wrap:wrap">
                    <select id="learn_dur" style="flex:1; min-width:120px">
                        <option value="60">1 min</option>
                        <option value="180" selected>3 min</option>
                        <option value="300">5 min</option>
                        <option value="600">10 min</option>
                        <option value="1800">30 min</option>
                        <option value="3600">60 min</option>
                        <option value="14400">4 hrs</option>
                        <option value="28800">8 hrs</option>
                    </select>
                    <button onclick="startLearn()" id="btn_learn" class="sec" style="flex:1" data-i18n="learn_static">📡 Learn Static</button>
                </div>
                <div id="learn_status" style="margin-top:6px; font-size:0.8rem; color:#888; display:none"></div>
            </div>
        </div>

        <!-- TAB 5: EVENTS -->
        <div id="tab5" class="hidden">
            <div style="display:flex; justify-content:space-between; align-items:center; margin-bottom:10px">
                <div class="section-title" style="margin:0; border:none" data-i18n="recent_events">RECENT EVENTS</div>
                <button onclick="clearEvents()" class="warn" style="width:auto; padding:5px 10px; margin:0" data-i18n="clear_history">Clear History</button>
            </div>
            <div style="overflow-x:auto">
                <table style="width:100%; border-collapse:collapse; font-size:0.8rem; text-align:left">
                    <thead>
                        <tr style="border-bottom:1px solid #444; color:#888">
                            <th style="padding:5px" data-i18n="evt_th_time">Time</th>
                            <th style="padding:5px" data-i18n="evt_th_type">Type</th>
                            <th style="padding:5px" data-i18n="evt_th_msg">Message</th>
                            <th style="padding:5px" data-i18n="evt_th_dist">Dist.</th>
                        </tr>
                    </thead>
                    <tbody id="event_list"></tbody>
                </table>
            </div>
        </div>
    </div>

    <!-- OTA -->
    <div class="card">
        <div class="stat-row"><span data-i18n="fw_update_title">FW Update</span></div>
        <input type="file" id="fw_file" accept=".bin">
        <div id="ota_bar" style="height:5px; background:#333; margin-top:5px; width:0%; transition:width 0.2s; background:var(--accent)"></div>
        <button onclick="uploadFW()" data-i18n="upload_fw">Upload Firmware</button>
    </div>
  </div>

  <div id="toast" data-i18n="saved">Saved</div>

<script>
// --- CORE ---
const $ = id => document.getElementById(id);
const api = (ep, opts={}) => {
    // ESPAsyncWebServer hasParam() only checks query params, not POST body
    // Convert URLSearchParams body to query string automatically
    if(opts.body instanceof URLSearchParams) {
        ep += (ep.includes('?') ? '&' : '?') + opts.body.toString();
        delete opts.body;
    }
    return fetch('/api/'+ep, opts).then(r => {
        if(r.ok) showToast(t('saved')); else showToast(t('save_error'));
        return r;
    });
};
function showToast(msg) { $('toast').innerText=msg; $('toast').style.opacity=1; setTimeout(()=>$('toast').style.opacity=0, 2000); }

// --- DATA STREAM ---
let histDist = new Array(60).fill(0);
let histMov = new Array(60).fill(0);
let histStat = new Array(60).fill(0);
let zones = [];
let gateResolution = 0.75, cfgMinGate = 0, cfgMaxGate = 13;

let evtSource = null;
let reconnectTimeout = null;

function connectSSE() {
    if (evtSource) {
        evtSource.close();
    }

    evtSource = new EventSource('/events');

    evtSource.addEventListener('telemetry', e => {
        const d = JSON.parse(e.data);
        updateUI(d);
        if(d.alarm_state) { alarmArmed = d.armed; updateAlarmUI(d.alarm_state); }
        if(d.gate_move && !$('tab2').classList.contains('hidden')) updateGatesUI(d);
    });

    evtSource.onerror = () => {
        console.log('SSE connection lost, reconnecting in 3s...');
        $('sse_icon').className = 'icon err';
        $('sse_icon').title = t('conn_lost');
        evtSource.close();
        if (reconnectTimeout) clearTimeout(reconnectTimeout);
        reconnectTimeout = setTimeout(connectSSE, 3000);
    };

    evtSource.onopen = () => {
        console.log('SSE connected');
        $('sse_icon').className = 'icon ok';
        $('sse_icon').title = t('realtime_ok');
    };
}

function init() {
    applyLang();

    // SSE Connection with auto-reconnect
    connectSSE();

    // Initial Load
    fetch('/api/version').then(r=>r.text()).then(v => $('fw_ver').innerText = v);

    fetch('/api/health').then(r=>r.json()).then(d => {
        if(d.is_default_pass) $('security_warning').style.display = 'block';
        if(d.auth_user) $('txt_auth_user').value = d.auth_user;
        if(d.hostname) $('txt_hostname').value = d.hostname;
        updateHealth(d);
    });

    // Load Configs
    loadMainConfig();
    loadSecurityConfig();
    loadMQTTConfig();
    loadStaticIPConfig();
    loadTelegramConfig();
    loadZones();
    loadAlarmStatus();

    initCollapsible();

    setInterval(() => fetch('/api/health').then(r=>r.json()).then(updateHealth), 5000);
}

function loadMainConfig() {
    fetch('/api/config').then(r=>r.json()).then(d => {
        $('i_max').value = d.max_gate;
        if(d.min_gate !== undefined) $('i_min').value = d.min_gate;
        $('i_hold').value = d.hold_time;
        if(d.led_en !== undefined) $('chk_led').checked = d.led_en;
        if(d.eng_mode !== undefined) $('chk_eng').checked = d.eng_mode;
        if(d.mov_sens && d.mov_sens.length > 0) $('i_sens').value = d.mov_sens[0]; // Display first gate sens as general
        if(d.resolution) gateResolution = d.resolution;
        if(d.min_gate !== undefined) cfgMinGate = d.min_gate;
        if(d.max_gate !== undefined) cfgMaxGate = d.max_gate;

        // Range summary
        let minDist = (cfgMinGate * gateResolution * 100).toFixed(0);
        let maxDist = (cfgMaxGate * gateResolution * 100).toFixed(0);
        $('range_summary').innerHTML = `${t('coverage')}: <b>${minDist}cm – ${maxDist}cm</b> &middot; ${t('resolution')}: ${gateResolution}m/gate`;

        // Gate Sliders (no energy bars — eng mode broken on V1.26)
        renderGateSliders(d.mov_sens, d.stat_sens);
    });
}

function updateUI(d) {
    // Sparklines
    histDist.push(d.distance_mm/10); histDist.shift();
    histMov.push(d.moving_energy); histMov.shift();
    histStat.push(d.static_energy); histStat.shift();

    drawSpark('graph_dist', histDist, 400); // max 400cm
    drawSpark('graph_mov', histMov, 100);
    drawSpark('graph_stat', histStat, 100);

    // Values
    $('dist_val').innerText = (d.distance_mm/10).toFixed(0);
    $('mov_val').innerText = d.moving_energy + '%';
    $('stat_val').innerText = d.static_energy + '%';

    let st = t('loading');
    let stColor = "#888";
    if(d.state === "detected") { st = t('detected'); stColor = "var(--accent)"; }
    else if(d.state === "hold") { st = t('hold'); stColor = "#bb86fc"; }
    else if(d.state === "idle") { st = t('idle'); stColor = "#888"; }
    if(d.tamper) { st = t('tamper_state'); stColor = "var(--warn)"; }
    $('state_text').innerText = st;
    $('state_text').style.color = stColor;
    drawZoneMap(d.raw_stat_dist, d.raw_mov_dist);
}

function renderGateSliders(mov, stat) {
    let h = '';
    for(let i=0; i<14; i++) {
        let dist = Math.round(i * gateResolution * 100);
        let active = (i >= cfgMinGate && i <= cfgMaxGate);
        let dimClass = active ? '' : ' gate-dimmed';
        let m = mov ? mov[i] : 50;
        let s = stat ? stat[i] : 30;
        h += `<div class="gate-wrapper${dimClass}">
            <div class="gate-label" style="width:65px; white-space:nowrap">G${i} <span style="color:#666">(${dist}cm)</span></div>
            <input type="range" class="mov-slider" id="g_m_${i}" value="${m}" min="0" max="100" title="Motion G${i}" oninput="$('lm_${i}').innerText=this.value" style="flex:1">
            <span id="lm_${i}" style="width:22px; text-align:right; color:#03dac6; font-size:0.75rem">${m}</span>
            <input type="range" class="stat-slider" id="g_s_${i}" value="${s}" min="0" max="100" title="Static G${i}" oninput="$('ls_${i}').innerText=this.value" style="flex:1">
            <span id="ls_${i}" style="width:22px; text-align:right; color:#bb86fc; font-size:0.75rem">${s}</span>
        </div>`;
    }
    $('gates_container').innerHTML = h;
}

function setAllGates() {
    let m = $('g_m_all').value, s = $('g_s_all').value;
    for(let i=0; i<14; i++) {
        let el_m = $(`g_m_${i}`), el_s = $(`g_s_${i}`);
        if(el_m) { el_m.value = m; $(`lm_${i}`).innerText = m; }
        if(el_s) { el_s.value = s; $(`ls_${i}`).innerText = s; }
    }
}

function updateGatesUI(d) {
    // No-op: energy bars removed (eng mode broken on V1.26 FW)
}

function updateHealth(d) {
    let ethOk = d.eth_link || (d.ethernet && d.ethernet.link_up);
    $('wifi_icon').className = "icon " + (ethOk ? "ok" : "err");
    $('mqtt_icon').className = "icon " + (d.mqtt && d.mqtt.connected ? "ok" : "err");
    $('h_score').innerText = d.health_score + "%";
    $('h_uart').innerText = d.uart_state;
    $('h_fps').innerText = d.frame_rate.toFixed(1) + " FPS";
    $('h_err').innerText = d.error_count;
    $('h_heap').innerText = (d.free_heap/1024).toFixed(1) + " / " + (d.min_heap/1024).toFixed(1) + " KB";
    if (d.chip_temp != null) $('h_temp').innerText = d.chip_temp.toFixed(1) + " °C";
    if ($('h_eth_link')) {
        let ethSpeed = (d.ethernet && d.ethernet.speed) ? d.ethernet.speed + " Mbps" : (ethOk ? "UP" : "DOWN");
        $('h_eth_link').innerText = ethOk ? "🟢 " + ethSpeed : "🔴 DOWN";
    }
    let u = d.uptime;
    $('h_uptime').innerText = Math.floor(u/3600) + "h " + Math.floor((u%3600)/60) + "m";
}

// --- GRAPHS ---
function drawSpark(id, data, max) {
    const el = $(id);
    let pts = "";
    const w = 100 / (data.length - 1);
    data.forEach((v, i) => {
        const y = 50 - (Math.min(v, max) / max * 50);
        pts += `${i * w},${y} `;
    });
    el.innerHTML = `<polyline points="${pts}" style="fill:none;stroke:inherit;stroke-width:2" />`;
}

// --- ACTIONS ---
function initCollapsible() {
    document.querySelectorAll('.section-title').forEach(el => {
        el.style.cursor = 'pointer';
        // Add icon/indicator
        el.innerHTML += ' <span style="font-size:0.8em; float:right">▼</span>';

        el.onclick = () => {
            let next = el.nextElementSibling;
            while(next && !next.classList.contains('section-title')) {
                next.style.display = next.style.display === 'none' ? '' : 'none';
                next = next.nextElementSibling;
            }
        };
    });
}

function tab(n) {
    ['tab0','tab1','tab2','tab3','tab4','tab5'].forEach((id, i) => {
        $(id).classList.toggle('hidden', i !== n);
        document.querySelectorAll('.tab')[i].classList.toggle('active', i === n);
    });
    // Refresh config when switching tabs
    if(n===1) { loadSecurityConfig(); loadAlarmStatus(); }
    if(n===2) loadMainConfig();
    if(n===5) loadEvents();
}

function loadEvents() {
    fetch('/api/events').then(r=>r.json()).then(d => {
        let h = '';
        d.forEach(e => {
            let typeColor = '#fff';
            let typeName = '?';
            // EVT_SYSTEM=0, PRESENCE=1, TAMPER=2, WIFI=3, HEARTBEAT=4, SECURITY=5
            switch(e.type) {
                case 0: typeName='SYS'; typeColor='#888'; break;
                case 1: typeName='MOV'; typeColor='#03dac6'; break;
                case 2: typeName='TMP'; typeColor='#cf6679'; break;
                case 3: typeName='NET'; typeColor='#bb86fc'; break;
                case 4: typeName='HB';  typeColor='#4caf50'; break;
                case 5: typeName='SEC'; typeColor='#ff9800'; break;
            }

            let u = e.ts;
            let timeStr;
            if (u > 1700000000) {
                // NTP epoch timestamp
                let dt = new Date(u * 1000);
                timeStr = dt.toLocaleString(LANG === 'cs' ? 'cs-CZ' : 'en-US', {day:'numeric',month:'numeric', hour:'2-digit',minute:'2-digit'});
            } else {
                // Uptime seconds (NTP not synced)
                timeStr = Math.floor(u/3600) + "h " + Math.floor((u%3600)/60) + "m";
            }

            h += `<tr style="border-bottom:1px solid #222">
                <td style="padding:5px; white-space:nowrap">${timeStr}</td>
                <td style="padding:5px; color:${typeColor}; font-weight:bold">${typeName}</td>
                <td style="padding:5px">${e.msg}</td>
                <td style="padding:5px">${e.dist > 0 ? (e.dist+"cm") : "-"}</td>
            </tr>`;
        });
        $('event_list').innerHTML = h || `<tr><td colspan="4" style="text-align:center; padding:10px; color:#666">${t('no_events')}</td></tr>`;
    });
}

function clearEvents() {
    if(confirm(t('del_history'))) {
        api('events/clear', {method:'POST'}).then(() => loadEvents());
    }
}

function startCalib() {
    if(confirm(t('noise_calib'))) {
        api('radar/calibrate', {method:'POST'});
    }
}

function saveBasic() {
    let m = $('i_max').value;
    let min = $('i_min').value;
    let h = $('i_hold').value;
    let s = $('i_sens').value;
    let l = $('chk_led').checked ? 1 : 0;
    api(`config`, {
        method: 'POST',
        body: new URLSearchParams({
            'gate': m,
            'min_gate': min,
            'hold': h,
            'mov': s,
            'led_en': l
        })
    });
}

function toggleEng() {
    let en = $('chk_eng').checked ? 1 : 0;
    api(`engineering`, {
        method: 'POST',
        body: new URLSearchParams({ 'enable': en })
    });
}

function loadSecurityConfig() {
    fetch('/api/security/config').then(r=>r.json()).then(d => {
        $('i_am').value = d.antimask_time || 300;
        $('chk_am_en').checked = d.antimask_enabled || false;
        $('i_loit').value = d.loiter_time || 15;
        $('chk_loit_en').checked = d.loiter_alert !== false;
        $('i_hb').value = d.heartbeat || 4;
        $('i_pet').value = d.pet_immunity || 0;
        $('i_rssi_thresh').value = d.rssi_threshold || -80;
        $('i_rssi_drop').value = d.rssi_drop || 20;
    }).catch(e => console.log('Security config not loaded'));

    // Load Light Config
    fetch('/api/radar/light').then(r=>r.json()).then(d => {
        if(d.function !== undefined) $('sel_light_func').value = d.function;
        if(d.threshold !== undefined) $('i_light_thresh').value = d.threshold;
        if(d.current_level !== undefined) $('cur_light_val').innerText = d.current_level;
    }).catch(e => console.log('Light config not loaded'));

    // Load Timeout (unmanned duration)
    fetch('/api/radar/timeout').then(r=>r.json()).then(d => {
        if(d.duration !== undefined) $('i_timeout').value = d.duration;
    }).catch(e => console.log('Timeout config not loaded'));
}

function saveLightConfig() {
    let func = $('sel_light_func').value;
    let thresh = $('i_light_thresh').value;
    api('radar/light', {
        method: 'POST',
        body: new URLSearchParams({ 'function': func, 'threshold': thresh })
    });
}

function saveTimeout() {
    let dur = $('i_timeout').value;
    api('radar/timeout', {
        method: 'POST',
        body: new URLSearchParams({ 'duration': dur })
    });
}

function saveSec() {
    let am = $('i_am').value;
    let am_en = $('chk_am_en').checked ? 1 : 0;
    let lo = $('i_loit').value;
    let lo_en = $('chk_loit_en').checked ? 1 : 0;
    let hb = $('i_hb').value;
    let pt = $('i_pet').value;
    let rt = $('i_rssi_thresh').value;
    let rd = $('i_rssi_drop').value;

    api(`security/config`, {
        method: 'POST',
        body: new URLSearchParams({
            'antimask': am,
            'antimask_en': am_en,
            'loiter': lo,
            'loiter_alert': lo_en,
            'heartbeat': hb,
            'pet': pt,
            'rssi_threshold': rt,
            'rssi_drop': rd
        })
    });
}

// MQTT Config
function loadMQTTConfig() {
    fetch('/api/health').then(r=>r.json()).then(d=>{
        if(d.mqtt) {
            $('chk_mqtt_en').checked = (d.mqtt.enabled !== false);
            $('txt_mqtt_server').value = d.mqtt.server || '';
            $('txt_mqtt_port').value = d.mqtt.port || '';
            $('txt_mqtt_user').value = d.mqtt.user || '';
        }
    });
}
function saveMQTTConfig() {
    let en = $('chk_mqtt_en').checked ? 1 : 0;
    let s = $('txt_mqtt_server').value;
    let p = $('txt_mqtt_port').value;
    let u = $('txt_mqtt_user').value;
    let pw = $('txt_mqtt_pass').value;

    api(`mqtt/config`, {
        method: 'POST',
        body: new URLSearchParams({
            'enabled': en,
            'server': s,
            'port': p,
            'user': u,
            'pass': pw
        })
    });
}

// Static IP Config (PoE-specific)
function loadStaticIPConfig() {
    fetch('/api/eth/config').then(r=>r.json()).then(d => {
        if(d.static_ip !== undefined) $('chk_static_ip').checked = d.static_ip;
        if(d.ip) $('txt_static_ip').value = d.ip;
        if(d.gateway) $('txt_static_gw').value = d.gateway;
        if(d.mask) $('txt_static_mask').value = d.mask;
        if(d.dns) $('txt_static_dns').value = d.dns;
    }).catch(e => console.log('ETH config not loaded'));
}
function saveStaticIP() {
    let en = $('chk_static_ip').checked ? 1 : 0;
    let ip = $('txt_static_ip').value;
    let gw = $('txt_static_gw').value;
    let mask = $('txt_static_mask').value;
    let dns = $('txt_static_dns').value;
    api('eth/config', {
        method: 'POST',
        body: new URLSearchParams({ 'static_ip': en, 'ip': ip, 'gateway': gw, 'mask': mask, 'dns': dns })
    });
}

// Telegram
function loadTelegramConfig() {
    fetch('/api/telegram/config').then(r=>r.json()).then(d => {
        $('chk_tg_en').checked = d.enabled;
        $('txt_tg_token').value = d.token || '';
        $('txt_tg_chat').value = d.chat_id || '';
    });
}
function saveTelegram() {
    let en = $('chk_tg_en').checked ? 1 : 0;
    let tk = $('txt_tg_token').value;
    let c = $('txt_tg_chat').value;
    api('telegram/config', {
        method: 'POST',
        body: new URLSearchParams({ 'enabled': en, 'token': tk, 'chat_id': c })
    });
}
function testTelegram() {
    fetch('/api/telegram/test', {method:'POST'})
    .then(r => r.json())
    .then(d => {
        showToast(d.success ? t('tg_ok') : t('tg_error') + ": " + (d.error || t('tg_unknown')));
    })
    .catch(e => showToast(t('comm_error')));
}

// Zones Implementation
function loadZones() {
    fetch('/api/zones').then(r=>r.json()).then(d => { zones = d; renderZones(); }).catch(e=>zones=[]);
}
const ZONE_COLORS = ['#1a6b3a','#1a4a6b','#6b1a1a','#4a1a6b'];
function renderZones() {
    let h = '';
    zones.forEach((z, i) => {
        const ab = z.alarm_behavior ?? 0;
        h += `<div style="margin-bottom:5px; background:#222; padding:5px; border-radius:5px; border-left:3px solid ${ZONE_COLORS[ab]||'#444'}">
            <div style="display:flex; gap:5px; margin-bottom:5px">
                <input type="text" value="${z.name}" id="z_name_${i}" style="flex:2" placeholder="${t('zone_name_ph')}">
                <input type="number" value="${z.min}" id="z_min_${i}" style="flex:1" placeholder="${t('zone_from')}">
                <input type="number" value="${z.max}" id="z_max_${i}" style="flex:1" placeholder="${t('zone_to')}">
            </div>
            <div style="display:flex; gap:5px; align-items:center">
                <select id="z_lvl_${i}" style="flex:1">
                    <option value="0" ${z.level==0?'selected':''}>Log</option>
                    <option value="1" ${z.level==1?'selected':''}>Info</option>
                    <option value="2" ${z.level==2?'selected':''}>Warn</option>
                    <option value="3" ${z.level==3?'selected':''}>ALARM</option>
                </select>
                <select id="z_ab_${i}" style="flex:2" title="${t('zone_path_title')}">
                    <option value="0" ${ab==0?'selected':''}>${t('zone_entry_delay')}</option>
                    <option value="1" ${ab==1?'selected':''}>${t('zone_immediate')}</option>
                    <option value="2" ${ab==2?'selected':''}>${t('zone_ignore')}</option>
                    <option value="3" ${ab==3?'selected':''}>${t('zone_ignore_static')}</option>
                </select>
                <input type="number" value="${z.delay||0}" id="z_del_${i}" style="flex:1" placeholder="Delay (ms)">
                <input type="checkbox" id="z_en_${i}" ${z.enabled!==false?'checked':''} style="width:auto">
                <button onclick="delZone(${i})" class="warn" style="width:auto; margin:0; padding:5px 10px">×</button>
            </div>
            <div style="display:flex; gap:5px; align-items:center">
                <span style="color:#888; font-size:0.75rem; white-space:nowrap">${t('zone_path_label')}</span>
                <select id="z_pz_${i}" style="flex:2" title="${t('zone_path_title')}">
                    <option value="" ${!z.prev_zone?'selected':''}>${t('zone_path_any')}</option>
                    ${zones.map((oz,j) => j!==i ? '<option value="'+oz.name+'" '+(z.prev_zone===oz.name?'selected':'')+'>'+oz.name+'</option>' : '').join('')}
                </select>
            </div>
        </div>`;
    });
    $('zones_list').innerHTML = h;
    drawZoneMap();
}
function addZone() {
    zones.push({name: t('zone_default') + " " + (zones.length+1), min: 0, max: 100, level: 0, alarm_behavior: 0, delay: 0, enabled: true, prev_zone: ""});
    renderZones();
}
function delZone(i) {
    zones.splice(i, 1);
    renderZones();
}
function saveZones() {
    let newZones = [];
    zones.forEach((_, i) => {
        newZones.push({
            name: document.getElementById(`z_name_${i}`).value,
            min: parseInt(document.getElementById(`z_min_${i}`).value),
            max: parseInt(document.getElementById(`z_max_${i}`).value),
            level: parseInt(document.getElementById(`z_lvl_${i}`).value),
            alarm_behavior: parseInt(document.getElementById(`z_ab_${i}`).value),
            delay: parseInt(document.getElementById(`z_del_${i}`).value),
            enabled: document.getElementById(`z_en_${i}`).checked,
            prev_zone: document.getElementById(`z_pz_${i}`).value
        });
    });
    zones = newZones;
    fetch('/api/zones', {
        method: 'POST',
        headers: {'Content-Type': 'application/json'},
        body: JSON.stringify(zones)
    }).then(r => {
        if(r.ok) showToast(t('zones_saved'));
        else showToast(t('save_error'));
    });
}

// ── Zone Map ───────────────────────────────────────────────────────────────────
function drawZoneMap(statDist, movDist) {
    const svg   = $('zone_map');
    const scale = $('zone_map_scale');
    if (!svg) return;
    const W = svg.clientWidth || 300, H = 48, MAX = 1050; // 14 gates × 75cm
    let html = '';
    zones.forEach(z => {
        if (!z.enabled) return;
        const ab  = z.alarm_behavior ?? 0;
        const col = ZONE_COLORS[ab] || '#444';
        const x1  = Math.round(z.min / MAX * W);
        const x2  = Math.round(z.max / MAX * W);
        html += `<rect x="${x1}" y="4" width="${x2-x1}" height="${H-8}" fill="${col}" opacity="0.5" rx="3"/>`;
        html += `<text x="${x1+3}" y="16" font-size="9" fill="#aaa">${z.name}</text>`;
    });
    if (statDist > 0) {
        const sx = Math.round(statDist / MAX * W);
        html += `<line x1="${sx}" y1="0" y2="${H}" x2="${sx}" stroke="#bb86fc" stroke-width="2"/>`;
    }
    if (movDist > 0) {
        const mx = Math.round(movDist / MAX * W);
        html += `<line x1="${mx}" y1="0" y2="${H}" x2="${mx}" stroke="#03dac6" stroke-width="2"/>`;
    }
    svg.innerHTML = html;
    if (scale) scale.innerText = '0cm' + ' '.repeat(10) + '525cm' + ' '.repeat(10) + '1050cm';
}

// ── Auto-learn ────────────────────────────────────────────────────────────────
let learnPollTimer = null;
function startLearn() {
    const dur = $('learn_dur').value;
    api(`radar/learn-static?duration=${dur}`, { method: 'POST' }).then(r => {
        if (r.ok) {
            $('btn_learn').disabled = true;
            $('learn_status').style.display = 'block';
            $('learn_status').innerText = t('starting');
            learnPollTimer = setInterval(pollLearn, 3000);
        }
    });
}
function pollLearn() {
    fetch('/api/radar/learn-static').then(r=>r.json()).then(d => {
        const stat = $('learn_status');
        if (!d.active && d.progress === 100) {
            clearInterval(learnPollTimer);
            $('btn_learn').disabled = false;
            let txt;
            if (d.suggest_ready) {
                txt = `${t('learn_done_zone')}: <b>${d.suggest_min_cm}–${d.suggest_max_cm}cm</b> <button onclick="applyLearnZone(${d.suggest_min_cm},${d.suggest_max_cm})" class="sec" style="padding:2px 8px; margin-left:6px">${t('learn_apply')}</button>`;
            } else {
                txt = `${t('learn_no_static')} (${d.static_samples} samples).`;
            }
            stat.innerHTML = txt;
        } else {
            stat.innerHTML = `${t('learn_running')}: ${d.progress}% &nbsp;|&nbsp; ${t('learn_static_pct')}: ${d.static_freq_pct}% &nbsp;|&nbsp; ${t('learn_top_gate')}: <b>${d.top_gate}</b> (~${d.top_cm}cm)`;
        }
    });
}
function applyLearnZone(minCm, maxCm) {
    zones.push({name: 'Statika-auto', min: minCm, max: maxCm, level: 0, alarm_behavior: 3, delay: 0, enabled: true});
    renderZones();
    $('learn_status').innerHTML += ` &nbsp;<b>${t('zone_added')}</b>`;
}

function saveGates() {
    let mov = [], stat = [];
    for(let i=0; i<14; i++) {
        mov.push(parseInt($(`g_m_${i}`).value));
        stat.push(parseInt($(`g_s_${i}`).value));
    }
    fetch('/api/radar/gates', {
        method: 'POST',
        headers: {'Content-Type': 'application/json'},
        body: JSON.stringify({mov, stat})
    }).then(r => {
        if(r.ok) showToast(t('gates_saved'));
        else showToast(t('gates_error'));
    });
}
function setPreset(type) {
    fetch('/api/preset?name=' + type, {method:'POST'}).then(r => {
        if(!r.ok) { showToast(t('preset_error')); return; }
        showToast(t('preset_applied') + ": " + type);
        // Re-fetch config and update sliders in-place (no reload)
        fetch('/api/config').then(r=>r.json()).then(d => {
            if(d.mov_sens && d.stat_sens) renderGateSliders(d.mov_sens, d.stat_sens);
        });
    });
}
function saveHostname() {
    let hn = $('txt_hostname').value;
    api(`config`, {
        method: 'POST',
        body: new URLSearchParams({ 'hostname': hn })
    });
}

function uploadFW() {
    let f = $('fw_file').files[0];
    if(!f) return;
    let fd = new FormData();
    fd.append('firmware', f);
    let xhr = new XMLHttpRequest();
    xhr.open('POST', '/api/update');
    xhr.upload.onprogress = e => $('ota_bar').style.width = (e.loaded/e.total*100) + "%";
    xhr.onload = () => alert(t('restarting'));
    xhr.send(fd);
}

// --- ALARM ---
let alarmArmed = false;
function loadAlarmStatus() {
    fetch('/api/alarm/status').then(r=>r.json()).then(d => {
        alarmArmed = d.armed;
        updateAlarmUI(d.state);
        $('i_entry_dl').value = d.entry_delay || 30;
        $('i_exit_dl').value = d.exit_delay || 30;
        $('chk_dis_rem').checked = d.disarm_reminder !== false;
    }).catch(()=>{});
}
function updateAlarmUI(state) {
    let badge = $('alarm_badge');
    let btn = $('btn_arm');
    if(state === 'disarmed') { badge.innerText = t('disarmed'); badge.style.color='#888'; btn.innerText=t('arm'); btn.style.background='#b00020'; }
    else if(state === 'arming') { badge.innerText = t('arming'); badge.style.color='orange'; btn.innerText=t('disarm'); btn.style.background='#3700b3'; }
    else if(state === 'armed_away') { badge.innerText = t('armed'); badge.style.color='#00ff00'; btn.innerText=t('disarm'); btn.style.background='#3700b3'; }
    else if(state === 'pending') { badge.innerText = t('pending'); badge.style.color='orange'; btn.innerText=t('disarm'); btn.style.background='#3700b3'; }
    else if(state === 'triggered') { badge.innerText = t('triggered'); badge.style.color='red'; btn.innerText=t('disarm'); btn.style.background='#3700b3'; }
}
function toggleArm() {
    if(alarmArmed) {
        api('alarm/disarm', {method:'POST'}).then(()=>{ alarmArmed=false; loadAlarmStatus(); });
    } else {
        api('alarm/arm', {method:'POST'}).then(()=>{ alarmArmed=true; loadAlarmStatus(); });
    }
}
function saveAlarmConfig() {
    let ed = $('i_entry_dl').value;
    let xd = $('i_exit_dl').value;
    let dr = $('chk_dis_rem').checked ? 1 : 0;
    api('alarm/config', {
        method: 'POST',
        body: new URLSearchParams({ 'entry_delay': ed, 'exit_delay': xd, 'disarm_reminder': dr })
    });
}

function saveAuth() {
    let u = $('txt_auth_user').value;
    let p = $('txt_auth_pass').value;
    let p2 = $('txt_auth_pass2').value;
    if(!u || !p) { showToast(t('enter_creds')); return; }
    if(p !== p2) { showToast(t('pass_mismatch')); return; }
    if(u.length < 4 || p.length < 4) { showToast(t('min_chars')); return; }

    fetch(`/api/auth/config?user=${encodeURIComponent(u)}&pass=${encodeURIComponent(p)}`, {
        method: 'POST'
    }).then(r => {
        if(r.ok) { showToast(t('pass_changed')); alert(t('creds_changed')); }
        else r.text().then(txt => showToast(txt || t('save_error')));
    });
}

window.onload = init;
</script>
</body>
</html>
)rawliteral";

#endif
