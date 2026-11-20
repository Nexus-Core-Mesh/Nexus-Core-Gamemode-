async function fetchJson(url) {
  const response = await fetch(url);
  if (!response.ok) {
    throw new Error('Request failed');
  }
  return response.json();
}

function renderStates(states) {
  const container = document.getElementById('states-container');
  if (!states.length) {
    container.innerHTML = '<div class="data-row"><span>Keine Daten verfügbar</span></div>';
    return;
  }

  container.innerHTML = states.map((state) => {
    const label = state.name || { 0: 'Los Santos', 1: 'San Fierro', 2: 'Las Venturas' }[state.id] || 'Unbekannt';
    const treasury = Number(state.treasury || 0);
    const taxRate = Number(state.tax_rate || 0);

    return `
      <div class="data-row">
        <strong>${label}</strong>
        <span>Budget ${treasury} · Steuer ${taxRate}%</span>
      </div>
    `;
  }).join('');
}

function renderPlayers(players) {
  const container = document.getElementById('players-container');
  if (!players.length) {
    container.innerHTML = '<div class="data-row"><span>Keine Daten verfügbar</span></div>';
    return;
  }

  container.innerHTML = players.map((player) => `
    <div class="data-row">
      <strong>${player.name}</strong>
      <span>State ${player.state_id || 0} · Geld ${player.money || 0}</span>
    </div>
  `).join('');
}

function renderAdmins(admins) {
  const container = document.getElementById('admins-container');
  if (!admins.length) {
    container.innerHTML = '<div class="data-row"><span>Keine Daten verfügbar</span></div>';
    return;
  }

  container.innerHTML = admins.map((admin) => `
    <div class="data-row">
      <strong>${admin.player_name}</strong>
      <span>Rang ${admin.rank || 0} · State ${admin.state_id || -1}</span>
    </div>
  `).join('');
}

function renderJobs(jobs) {
  const container = document.getElementById('jobs-container');
  if (!jobs.length) {
    container.innerHTML = '<div class="data-row"><span>Keine Daten verfügbar</span></div>';
    return;
  }

  container.innerHTML = jobs.map((job) => `
    <div class="data-row">
      <strong>${job.name}</strong>
      <span>Type ${job.type || 0} · State ${job.state_id || 0}</span>
    </div>
  `).join('');
}

function renderFactions(factions) {
  const container = document.getElementById('factions-container');
  if (!factions.length) {
    container.innerHTML = '<div class="data-row"><span>Keine Daten verfügbar</span></div>';
    return;
  }

  container.innerHTML = factions.map((faction) => `
    <div class="data-row">
      <strong>${faction.name || 'Faction'}</strong>
      <span>State ${faction.state_id || 0} · Typ ${faction.type || 0}</span>
    </div>
  `).join('');
}

function renderHouses(houses) {
  const container = document.getElementById('houses-container');
  if (!houses.length) {
    container.innerHTML = '<div class="data-row"><span>Keine Daten verfügbar</span></div>';
    return;
  }

  container.innerHTML = houses.map((house) => `
    <div class="data-row">
      <strong>${house.name || 'Haus'}</strong>
      <span>State ${house.state_id || 0} · Besitzer ${house.owner_id ?? 'N/A'}</span>
    </div>
  `).join('');
}

function renderBusinesses(businesses) {
  const container = document.getElementById('businesses-container');
  if (!businesses.length) {
    container.innerHTML = '<div class="data-row"><span>Keine Daten verfügbar</span></div>';
    return;
  }

  container.innerHTML = businesses.map((biz) => `
    <div class="data-row">
      <strong>${biz.name || 'Business'}</strong>
      <span>State ${biz.state_id || 0} · Umsatz ${biz.revenue || 0}</span>
    </div>
  `).join('');
}

function renderSeason(seasonData) {
  const container = document.getElementById('season-container');
  const season = seasonData?.season || {};
  const events = season.events || [];
  const states = season.states || [];

  const rows = [];
  if (states.length) {
    rows.push(...states.map((state) => `
      <div class="data-row">
        <strong>${state.name || state.short_name || 'State'}</strong>
        <span>Budget ${state.treasury || 0} · Steuer ${state.tax_rate || 0}%</span>
      </div>
    `));
  }
  if (events.length) {
    rows.push(...events.slice(0, 4).map((event) => `
      <div class="data-row">
        <strong>${event.name || 'Event'}</strong>
        <span>${event.type || 'season'} · ${event.value || 0}</span>
      </div>
    `));
  }
  if (!rows.length) {
    container.innerHTML = '<div class="data-row"><span>Keine Season-/State-Daten verfügbar</span></div>';
    return;
  }

  container.innerHTML = rows.join('');
}

async function loadDashboard() {
  try {
    const [overview, info, season] = await Promise.all([
      fetchJson('/api/overview'),
      fetchJson('/api/server-info'),
      fetchJson('/api/season').catch(() => ({ season: { events: [], states: [] } }))
    ]);

    document.getElementById('server-status').textContent = info.status || 'online';
    document.getElementById('server-project').textContent = info.project || 'Nexus Core';
    document.getElementById('state-count').textContent = info.stateCount || overview.states?.length || 0;
    document.getElementById('player-count').textContent = info.playerCount || overview.players?.length || 0;
    document.getElementById('admin-count').textContent = info.adminCount || overview.admins?.length || 0;
    document.getElementById('job-count').textContent = info.jobCount || overview.jobs?.length || 0;
    document.getElementById('treasury-total').textContent = `${info.treasuryTotal || 0}$`;

    renderStates(overview.states || []);
    renderPlayers(overview.players || []);
    renderAdmins(overview.admins || []);
    renderJobs(overview.jobs || []);
    renderFactions(overview.factions || []);
    renderHouses(overview.houses || []);
    renderBusinesses(overview.businesses || []);
    renderSeason(season);
  } catch (error) {
    document.getElementById('server-status').textContent = 'offline';
    document.getElementById('server-project').textContent = 'Datenbank nicht erreichbar';
  }
}

document.getElementById('refreshBtn').addEventListener('click', loadDashboard);
loadDashboard();
