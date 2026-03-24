import React, { useState, useEffect } from 'react';
import './App.css';

const ESP_IP = "/api";

const CIRC = 2 * Math.PI * 45;

function HumidityRing({ value }) {
  const offset = CIRC - (value / 100) * CIRC;
  return (
    <div className="ring-cont">
      <svg width="120" height="120" viewBox="0 0 120 120">
        <defs>
          <linearGradient id="gradRing" x1="0%" y1="0%" x2="100%" y2="0%">
            <stop offset="0%"   stopColor="#00ffaa" />
            <stop offset="100%" stopColor="#00b4ff" />
          </linearGradient>
        </defs>
        <circle className="ring-bg"   cx="60" cy="60" r="45" />
        <circle
          className="ring-fill"
          cx="60" cy="60" r="45"
          strokeDasharray={CIRC}
          strokeDashoffset={offset}
        />
      </svg>
      <div className="ring-num">
        <span className="ring-val">{value}</span>
        <span className="ring-unit">HUM %</span>
      </div>
    </div>
  );
}

export default function App() {
  const [data, setData] = useState({
    humidity: 0,
    valve: false,
    timerActive: false,
    autoMode: false,
  });
  const [activeSeconds, setActiveSeconds] = useState(null);

  const fetchStatus = async () => {
    try {
      const res  = await fetch(`${ESP_IP}/status`);
      const json = await res.json();
      setData(json);
      if (!json.timerActive) setActiveSeconds(null);
    } catch (e) {
      console.error("ESP Offline");
    }
  };

  const startTimer = async (segundos) => {
    setActiveSeconds(segundos);
    await fetch(`${ESP_IP}/startTimer?seg=${segundos}`);
    fetchStatus();
  };

  const setManual = async (state) => {
    await fetch(`${ESP_IP}/manual?state=${state}`);
    fetchStatus();
  };

  const toggleAutoMode = async () => {
    await fetch(`${ESP_IP}/setAuto?active=${!data.autoMode}`);
    fetchStatus();
  };

  useEffect(() => {
    fetchStatus();
    const id = setInterval(fetchStatus, 2000);
    return () => clearInterval(id);
  }, []);

  return (
    <div className="dashboard-container">

      {/* ── HEADER ── */}
      <header className="header">
        <h1>SISTEMA DE RIEGO</h1>
        <div className="live-badge">
          <span className="live-dot" />
          EN LÍNEA · ACTUALIZANDO CADA 2S
        </div>
      </header>

      <div className="grid">

        {/* ── HUMEDAD ── */}
        <div className="card">
          <span className="card-label">Humedad del suelo</span>
          <div className="hum-wrap">
            <HumidityRing value={data.humidity} />
            <span className={`status-pill ${data.valve ? 'on' : 'off'}`}>
              {data.valve ? '🌊 VÁLVULA ABIERTA' : '💤 VÁLVULA CERRADA'}
            </span>
          </div>
        </div>

        {/* ── MANUAL ── */}
        <div className="card">
          <span className="card-label">Control manual</span>
          <div className="manual-controls">
            <button
              onClick={() => setManual('ON')}
              className={`btn-manual btn-on ${data.valve ? 'active' : ''}`}
            >
              ▶ ABRIR
            </button>
            <button
              onClick={() => setManual('OFF')}
              className={`btn-manual btn-off ${!data.valve ? 'active' : ''}`}
            >
              ■ CERRAR
            </button>
          </div>
        </div>

        {/* ── TIMER ── */}
        <div className="card full-width">
          <span className="card-label">Temporizador de riego</span>
          <div className="timer-grid">
            {[600, 1200, 1800, 2700].map(s => (
              <button
                key={s}
                onClick={() => startTimer(s)}
                className={`btn-timer ${
                  data.timerActive && data.valve && activeSeconds === s
                    ? 'timer-running' : ''
                }`}
              >
                {s / 60} MIN
              </button>
            ))}
          </div>
          {data.timerActive && (
            <p className="timer-msg">⏳ RIEGO AUTOMÁTICO EN CURSO</p>
          )}
        </div>

        {/* ── AUTO ── */}
        <div className="card full-width">
          <span className="card-label">Modo automático</span>
          <div className="auto-inner">
            <span className="auto-icon">{data.autoMode ? '🤖' : '😴'}</span>
            <div className="auto-right">
              <p className="auto-desc">
                {data.autoMode
                  ? '▸ ACTIVO — el sistema riega automáticamente cuando la humedad baja del 30%.'
                  : '▸ INACTIVO — controla el riego de forma manual o usa el temporizador.'}
              </p>
              <button
                onClick={toggleAutoMode}
                className={`btn-control ${data.autoMode ? 'btn-on' : 'btn-off'}`}
              >
                {data.autoMode ? '✓ AUTO ACTIVO' : '⊘ AUTO INACTIVO'}
              </button>
            </div>
          </div>
        </div>

      </div>

      <footer className="footer">
        SISTEMA DE RIEGO INTELIGENTE · DIEGO BRAVO · {new Date().getFullYear()}
      </footer>
    </div>
  );
}