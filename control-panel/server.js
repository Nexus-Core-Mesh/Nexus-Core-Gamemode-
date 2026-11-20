const express = require('express');
const path = require('path');
const sqlite3 = require('sqlite3').verbose();

const app = express();
const port = process.env.PORT || 3001;
const dbPath = path.resolve(__dirname, '../data/db/gtasa_reallife.db');

const stateNameMap = {
  0: 'Los Santos',
  1: 'San Fierro',
  2: 'Las Venturas'
};

app.use(express.json());
app.use(express.static(path.join(__dirname, 'public')));

const db = new sqlite3.Database(dbPath, sqlite3.OPEN_READONLY, (err) => {
  if (err) {
    console.error('DB connection failed:', err.message);
  } else {
    console.log('Connected to SQLite database:', dbPath);
  }
});

function tableExists(tableName) {
  return new Promise((resolve, reject) => {
    db.get(
      "SELECT name FROM sqlite_master WHERE type = 'table' AND name = ?",
      [tableName],
      (err, row) => {
        if (err) return reject(err);
        resolve(Boolean(row));
      }
    );
  });
}

function queryRows(sql, params = []) {
  return new Promise((resolve, reject) => {
    db.all(sql, params, (err, rows) => {
      if (err) return reject(err);
      resolve(rows || []);
    });
  });
}

function querySingle(sql, params = []) {
  return new Promise((resolve, reject) => {
    db.get(sql, params, (err, row) => {
      if (err) return reject(err);
      resolve(row || null);
    });
  });
}

async function safeTableRows(tableName, selectSql, fallbackLimit = 25) {
  const exists = await tableExists(tableName);
  if (!exists) return [];
  return queryRows(selectSql, [], fallbackLimit);
}

function normalizeStateName(stateId, fallbackName) {
  if (!Number.isFinite(Number(stateId))) {
    return fallbackName || 'Unbekannt';
  }
  const id = Number(stateId);
  return stateNameMap[id] || fallbackName || 'Unbekannt';
}

async function getServerSummary() {
  const tables = await queryRows("SELECT name FROM sqlite_master WHERE type = 'table' ORDER BY name");
  const stateCount = await querySingle('SELECT COUNT(*) AS count FROM states');
  const playerCount = await querySingle('SELECT COUNT(*) AS count FROM players');
  const adminCount = await querySingle('SELECT COUNT(*) AS count FROM admin_roles');
  const jobCount = await querySingle('SELECT COUNT(*) AS count FROM jobs');
  const stateSum = await querySingle('SELECT COALESCE(SUM(treasury), 0) AS total FROM states');
  const bankSum = await querySingle('SELECT COALESCE(SUM(balance), 0) AS total FROM state_bank');
  const stateBudgetRows = await tableExists('states') ? queryRows('SELECT id, treasury, tax_rate, name FROM states ORDER BY id ASC') : [];

  const stateBudgetSummary = stateBudgetRows.map((row) => ({
    id: row.id,
    name: normalizeStateName(row.id, row.name),
    treasury: Number(row.treasury || 0),
    taxRate: Number(row.tax_rate || 0)
  }));

  return {
    tables: tables.map((row) => row.name),
    stateCount: stateCount ? stateCount.count : 0,
    playerCount: playerCount ? playerCount.count : 0,
    adminCount: adminCount ? adminCount.count : 0,
    jobCount: jobCount ? jobCount.count : 0,
    treasuryTotal: stateSum ? stateSum.total : 0,
    bankTotal: bankSum ? bankSum.total : 0,
    stateBudgets: stateBudgetSummary,
    project: 'Nexus Core Gamemode',
    panelVersion: '1.1.0'
  };
}

app.get('/api/server-info', async (req, res) => {
  try {
    const summary = await getServerSummary();
    res.json({ status: 'online', database: dbPath, ...summary });
  } catch (error) {
    res.status(500).json({ error: error.message });
  }
});

app.get('/api/summary', async (req, res) => {
  try {
    const summary = await getServerSummary();
    const states = await tableExists('states') ? queryRows('SELECT * FROM states ORDER BY id ASC') : [];
    const players = await tableExists('players') ? queryRows('SELECT * FROM players ORDER BY id ASC LIMIT 20') : [];
    const admins = await tableExists('admin_roles') ? queryRows('SELECT * FROM admin_roles ORDER BY id ASC LIMIT 20') : [];
    const jobs = await tableExists('jobs') ? queryRows('SELECT * FROM jobs ORDER BY id ASC LIMIT 20') : [];
    const factions = await tableExists('factions') ? queryRows('SELECT * FROM factions ORDER BY id ASC LIMIT 20') : [];
    const houses = await tableExists('houses') ? queryRows('SELECT * FROM houses ORDER BY id ASC LIMIT 20') : [];
    const businesses = await tableExists('businesses') ? queryRows('SELECT * FROM businesses ORDER BY id ASC LIMIT 20') : [];

    res.json({
      ...summary,
      states,
      players,
      admins,
      jobs,
      factions,
      houses,
      businesses,
      generatedAt: new Date().toISOString()
    });
  } catch (error) {
    res.status(500).json({ error: error.message });
  }
});

app.get('/api/overview', async (req, res) => {
  try {
    const summary = await getServerSummary();
    const states = await tableExists('states') ? queryRows('SELECT * FROM states ORDER BY id ASC') : [];
    const players = await tableExists('players') ? queryRows('SELECT * FROM players ORDER BY id ASC LIMIT 20') : [];
    const admins = await tableExists('admin_roles') ? queryRows('SELECT * FROM admin_roles ORDER BY id ASC LIMIT 20') : [];
    const jobs = await tableExists('jobs') ? queryRows('SELECT * FROM jobs ORDER BY id ASC LIMIT 20') : [];
    const factions = await tableExists('factions') ? queryRows('SELECT * FROM factions ORDER BY id ASC LIMIT 20') : [];
    const houses = await tableExists('houses') ? queryRows('SELECT * FROM houses ORDER BY id ASC LIMIT 20') : [];
    const businesses = await tableExists('businesses') ? queryRows('SELECT * FROM businesses ORDER BY id ASC LIMIT 20') : [];

    res.json({
      states,
      players,
      admins,
      jobs,
      factions,
      houses,
      businesses,
      summary,
      generatedAt: new Date().toISOString()
    });
  } catch (error) {
    res.status(500).json({ error: error.message });
  }
});

app.get('/api/states', async (req, res) => {
  try {
    const rows = await tableExists('states') ? queryRows('SELECT * FROM states ORDER BY id ASC') : [];
    res.json(rows.map((row) => ({
      ...row,
      name: normalizeStateName(row.id, row.name),
      short_name: row.short_name || ['LS', 'SF', 'LV'][Number(row.id)] || 'UNK'
    })));
  } catch (error) {
    res.status(500).json({ error: error.message });
  }
});

app.get('/api/players', async (req, res) => {
  try {
    const rows = await tableExists('players') ? queryRows('SELECT * FROM players ORDER BY id ASC LIMIT 50') : [];
    res.json(rows);
  } catch (error) {
    res.status(500).json({ error: error.message });
  }
});

app.get('/api/admins', async (req, res) => {
  try {
    const rows = await tableExists('admin_roles') ? queryRows('SELECT * FROM admin_roles ORDER BY id ASC LIMIT 50') : [];
    res.json(rows);
  } catch (error) {
    res.status(500).json({ error: error.message });
  }
});

app.get('/api/jobs', async (req, res) => {
  try {
    const rows = await tableExists('jobs') ? queryRows('SELECT * FROM jobs ORDER BY id ASC LIMIT 50') : [];
    res.json(rows);
  } catch (error) {
    res.status(500).json({ error: error.message });
  }
});

app.get('/api/factions', async (req, res) => {
  try {
    const rows = await tableExists('factions') ? queryRows('SELECT * FROM factions ORDER BY id ASC LIMIT 50') : [];
    res.json(rows);
  } catch (error) {
    res.status(500).json({ error: error.message });
  }
});

app.get('/api/houses', async (req, res) => {
  try {
    const rows = await tableExists('houses') ? queryRows('SELECT * FROM houses ORDER BY id ASC LIMIT 50') : [];
    res.json(rows);
  } catch (error) {
    res.status(500).json({ error: error.message });
  }
});

app.get('/api/businesses', async (req, res) => {
  try {
    const rows = await tableExists('businesses') ? queryRows('SELECT * FROM businesses ORDER BY id ASC LIMIT 50') : [];
    res.json(rows);
  } catch (error) {
    res.status(500).json({ error: error.message });
  }
});

app.get('/api/season', async (req, res) => {
  try {
    const rows = await tableExists('server_events') ? queryRows('SELECT * FROM server_events ORDER BY id ASC LIMIT 50') : [];
    const states = await tableExists('states') ? queryRows('SELECT id, treasury, tax_rate, name FROM states ORDER BY id ASC') : [];
    res.json({
      season: {
        source: 'sqlite-readonly',
        note: 'Season state is sourced from the runtime layer and mirrored to the panel as operational status; it is not the primary save authority.',
        events: rows,
        states: states.map((row) => ({
          ...row,
          name: normalizeStateName(row.id, row.name),
          short_name: row.short_name || ['LS', 'SF', 'LV'][Number(row.id)] || 'UNK'
        }))
      },
      generatedAt: new Date().toISOString()
    });
  } catch (error) {
    res.status(500).json({ error: error.message });
  }
});

app.get('*', (req, res) => {
  res.sendFile(path.join(__dirname, 'public', 'index.html'));
});

process.on('SIGINT', () => {
  db.close();
  process.exit(0);
});

app.listen(port, () => {
  console.log(`Control panel running on http://localhost:${port}`);
});
