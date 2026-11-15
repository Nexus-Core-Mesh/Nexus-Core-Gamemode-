CREATE TABLE IF NOT EXISTS states (
    id INTEGER PRIMARY KEY,
    name TEXT NOT NULL,
    short_name TEXT NOT NULL,
    treasury INTEGER NOT NULL DEFAULT 0,
    tax_rate REAL NOT NULL DEFAULT 0.0,
    created_at TEXT NOT NULL DEFAULT (datetime('now'))
);

CREATE TABLE IF NOT EXISTS state_bank (
    state_id INTEGER PRIMARY KEY,
    balance INTEGER NOT NULL DEFAULT 0,
    updated_at TEXT NOT NULL DEFAULT (datetime('now')),
    FOREIGN KEY (state_id) REFERENCES states(id)
);

CREATE TABLE IF NOT EXISTS players (
    id INTEGER PRIMARY KEY AUTOINCREMENT,
    name TEXT NOT NULL UNIQUE,
    state_id INTEGER NOT NULL DEFAULT 0,
    money INTEGER NOT NULL DEFAULT 0,
    admin_level INTEGER NOT NULL DEFAULT 0,
    admin_state INTEGER NOT NULL DEFAULT -1,
    home_state INTEGER NOT NULL DEFAULT 0,
    phone_number TEXT DEFAULT 'NONE',
    has_health_insurance INTEGER NOT NULL DEFAULT 0,
    created_at TEXT NOT NULL DEFAULT (datetime('now'))
);

CREATE TABLE IF NOT EXISTS player_state (
    player_id INTEGER PRIMARY KEY,
    state_id INTEGER NOT NULL DEFAULT 0,
    last_updated TEXT NOT NULL DEFAULT (datetime('now')),
    FOREIGN KEY (player_id) REFERENCES players(id)
);

CREATE TABLE IF NOT EXISTS admin_roles (
    id INTEGER PRIMARY KEY AUTOINCREMENT,
    player_name TEXT NOT NULL,
    rank INTEGER NOT NULL DEFAULT 0,
    state_id INTEGER NOT NULL DEFAULT -1,
    active INTEGER NOT NULL DEFAULT 1,
    UNIQUE(player_name, state_id, rank)
);

CREATE TABLE IF NOT EXISTS jobs (
    id INTEGER PRIMARY KEY AUTOINCREMENT,
    name TEXT NOT NULL UNIQUE,
    type INTEGER NOT NULL DEFAULT 0,
    state_id INTEGER NOT NULL DEFAULT 0,
    enabled INTEGER NOT NULL DEFAULT 1
);

CREATE TABLE IF NOT EXISTS player_inventory (
    id INTEGER PRIMARY KEY AUTOINCREMENT,
    player_id INT NOT NULL,
    item_id INT NOT NULL,
    slot_index INT NOT NULL,
    quantity INT NOT NULL DEFAULT 1,
    metadata VARCHAR(128) DEFAULT 'NONE',
    FOREIGN KEY(player_id) REFERENCES players(id)
);

CREATE TABLE IF NOT EXISTS player_accessories (
    id INTEGER PRIMARY KEY AUTOINCREMENT,
    player_id INT NOT NULL,
    accessory_type INT NOT NULL DEFAULT 0,
    color INT NOT NULL DEFAULT 0,
    equipped INT NOT NULL DEFAULT 0,
    updated_at TEXT NOT NULL DEFAULT (datetime('now')),
    FOREIGN KEY(player_id) REFERENCES players(id)
);

CREATE TABLE IF NOT EXISTS player_properties (
    id INTEGER PRIMARY KEY AUTOINCREMENT,
    player_id INTEGER NOT NULL,
    property_type INTEGER NOT NULL DEFAULT 0,
    property_id INTEGER NOT NULL DEFAULT 0,
    state_id INTEGER NOT NULL DEFAULT 0,
    value INTEGER NOT NULL DEFAULT 0,
    created_at TEXT NOT NULL DEFAULT (datetime('now')),
    FOREIGN KEY(player_id) REFERENCES players(id)
);

CREATE TABLE IF NOT EXISTS fuel_stations (
    id INTEGER PRIMARY KEY AUTOINCREMENT,
    state_id INT NOT NULL DEFAULT 0,
    x REAL NOT NULL DEFAULT 0.0,
    y REAL NOT NULL DEFAULT 0.0,
    z REAL NOT NULL DEFAULT 0.0,
    price INT NOT NULL DEFAULT 10,
    updated_at TEXT NOT NULL DEFAULT (datetime('now'))
);

CREATE TABLE IF NOT EXISTS house_storage (
    id INTEGER PRIMARY KEY AUTOINCREMENT,
    house_id INT NOT NULL,
    state_id INT NOT NULL DEFAULT 0,
    owner_player_id INT DEFAULT NULL,
    safe_type INT NOT NULL DEFAULT 0,
    money INT NOT NULL DEFAULT 0,
    updated_at TEXT NOT NULL DEFAULT (datetime('now')),
    FOREIGN KEY(owner_player_id) REFERENCES players(id)
);

CREATE TABLE IF NOT EXISTS house_storage_items (
    id INTEGER PRIMARY KEY AUTOINCREMENT,
    house_id INT NOT NULL,
    item_id INT NOT NULL,
    slot_index INT NOT NULL,
    quantity INT NOT NULL DEFAULT 0,
    metadata VARCHAR(128) DEFAULT 'NONE',
    updated_at TEXT NOT NULL DEFAULT (datetime('now'))
);

CREATE TABLE IF NOT EXISTS ubb_vehicles (
    id INTEGER PRIMARY KEY AUTOINCREMENT,
    vehicle_id INT NOT NULL,
    model_id INT NOT NULL,
    owner_player_id INT DEFAULT NULL,
    state_id INT NOT NULL DEFAULT 0,
    fuel INT NOT NULL DEFAULT 100,
    price INT NOT NULL DEFAULT 0,
    status INT NOT NULL DEFAULT 0,
    updated_at TEXT NOT NULL DEFAULT (datetime('now'))
);

CREATE TABLE IF NOT EXISTS player_contacts (
    id INTEGER PRIMARY KEY AUTOINCREMENT,
    player_id INT NOT NULL,
    contact_name VARCHAR(32) NOT NULL,
    phone_number VARCHAR(32) NOT NULL,
    created_at TEXT NOT NULL DEFAULT (datetime('now')),
    FOREIGN KEY(player_id) REFERENCES players(id)
);

CREATE TABLE IF NOT EXISTS criminal_records (
    id INTEGER PRIMARY KEY AUTOINCREMENT,
    player_id INT NOT NULL,
    state_id INT NOT NULL DEFAULT 0,
    offense TEXT NOT NULL,
    wanted_level INT NOT NULL DEFAULT 0,
    bail_amount INT NOT NULL DEFAULT 0,
    prison_seconds INT NOT NULL DEFAULT 0,
    officer_name VARCHAR(32) DEFAULT 'SYSTEM',
    created_at TEXT NOT NULL DEFAULT (datetime('now')),
    FOREIGN KEY(player_id) REFERENCES players(id)
);

CREATE TABLE IF NOT EXISTS player_vehicles (
    id INTEGER PRIMARY KEY AUTOINCREMENT,
    player_id INTEGER NOT NULL,
    model_id INTEGER NOT NULL,
    state_id INTEGER NOT NULL DEFAULT 0,
    price_paid INTEGER NOT NULL DEFAULT 0,
    fuel INTEGER NOT NULL DEFAULT 100,
    locked INTEGER NOT NULL DEFAULT 1,
    pos_x REAL DEFAULT 0.0,
    pos_y REAL DEFAULT 0.0,
    pos_z REAL DEFAULT 0.0,
    pos_a REAL DEFAULT 0.0,
    health REAL DEFAULT 1000.0,
    color1 INTEGER DEFAULT 0,
    color2 INTEGER DEFAULT 0,
    created_at TEXT NOT NULL DEFAULT (datetime('now')),
    FOREIGN KEY(player_id) REFERENCES players(id)
);

CREATE TABLE IF NOT EXISTS vehicle_mods (
    id INTEGER PRIMARY KEY AUTOINCREMENT,
    vehicle_id INT NOT NULL,
    component_id INT NOT NULL,
    mod_id INT NOT NULL DEFAULT 0,
    value INT NOT NULL DEFAULT 0,
    updated_at TEXT NOT NULL DEFAULT (datetime('now'))
);

CREATE TABLE IF NOT EXISTS houses (
    id INTEGER PRIMARY KEY AUTOINCREMENT,
    owner_player_id INTEGER DEFAULT NULL,
    state_id INTEGER NOT NULL DEFAULT 0,
    name TEXT NOT NULL,
    price INTEGER NOT NULL DEFAULT 0,
    x REAL NOT NULL DEFAULT 0.0,
    y REAL NOT NULL DEFAULT 0.0,
    z REAL NOT NULL DEFAULT 0.0,
    locked INTEGER NOT NULL DEFAULT 1,
    created_at TEXT NOT NULL DEFAULT (datetime('now')),
    FOREIGN KEY(owner_player_id) REFERENCES players(id)
);

CREATE TABLE IF NOT EXISTS businesses (
    id INTEGER PRIMARY KEY AUTOINCREMENT,
    state_id INTEGER NOT NULL DEFAULT 0,
    name TEXT NOT NULL,
    owner_player_id INTEGER DEFAULT NULL,
    type TEXT NOT NULL DEFAULT 'retail',
    value INTEGER NOT NULL DEFAULT 0,
    income INTEGER NOT NULL DEFAULT 0,
    created_at TEXT NOT NULL DEFAULT (datetime('now')),
    FOREIGN KEY(owner_player_id) REFERENCES players(id)
);

CREATE TABLE IF NOT EXISTS factions (
    id INTEGER PRIMARY KEY AUTOINCREMENT,
    state_id INTEGER NOT NULL DEFAULT 0,
    name TEXT NOT NULL,
    type TEXT NOT NULL DEFAULT 'gang',
    leader_player_id INTEGER DEFAULT NULL,
    treasury INTEGER NOT NULL DEFAULT 0,
    created_at TEXT NOT NULL DEFAULT (datetime('now')),
    FOREIGN KEY(leader_player_id) REFERENCES players(id)
);

CREATE TABLE IF NOT EXISTS web_account_sync (
    id INTEGER PRIMARY KEY AUTOINCREMENT,
    player_name TEXT NOT NULL,
    ucp_username TEXT NOT NULL,
    password_hash TEXT NOT NULL,
    synced_at TEXT NOT NULL DEFAULT (datetime('now'))
);

CREATE TABLE IF NOT EXISTS server_events (
    id INTEGER PRIMARY KEY AUTOINCREMENT,
    name TEXT NOT NULL,
    state_id INTEGER NOT NULL DEFAULT 0,
    enabled INTEGER NOT NULL DEFAULT 1,
    started_at TEXT NOT NULL DEFAULT (datetime('now'))
);

INSERT OR IGNORE INTO states (id, name, short_name, treasury, tax_rate) VALUES
    (0, 'Los Santos', 'LS', 500000, 8.5),
    (1, 'San Fierro', 'SF', 450000, 7.5),
    (2, 'Las Venturas', 'LV', 400000, 9.0);

INSERT OR IGNORE INTO state_bank (state_id, balance) VALUES
    (0, 500000),
    (1, 450000),
    (2, 400000);

INSERT OR IGNORE INTO jobs (name, type, state_id, enabled) VALUES
    ('Truckfahrer', 4, 0, 1),
    ('Zivilist', 1, 0, 1),
    ('Polizei', 2, 0, 1),
    ('Sanitäter', 3, 0, 1),
    ('Jobservice', 1, 1, 1),
    ('Staatsdienst', 2, 1, 1),
    ('Stadtservice', 1, 2, 1);

INSERT OR IGNORE INTO houses (id, state_id, name, price, x, y, z, locked) VALUES
    (1, 0, 'Downtown Villa', 40000, 0.0, 0.0, 0.0, 1),
    (2, 1, 'SF Loft', 35000, 0.0, 0.0, 0.0, 1),
    (3, 2, 'LV Penthouse', 55000, 0.0, 0.0, 0.0, 1);

INSERT OR IGNORE INTO businesses (id, state_id, name, type, value, income) VALUES
    (1, 0, 'Los Santos Market', 'retail', 25000, 800),
    (2, 1, 'SF Import Outlet', 'retail', 22000, 650),
    (3, 2, 'LV Casino Lounge', 'casino', 50000, 1200);

INSERT OR IGNORE INTO factions (id, state_id, name, type, treasury) VALUES
    (1, 0, 'Ballas', 'gang', 15000),
    (2, 1, 'Vagos', 'gang', 12000),
    (3, 2, 'The Family', 'gang', 18000);
