#include "esp.h"
const express = require('express');
const fs = require('fs');
const path = require('path');

const app = express();
const PORTA = process.env.PORT || 3000;

app.use(express.json());
app.use(express.static(__dirname));

const CONFIG = path.join(__dirname, 'config.json');

// Cria o arquivo de config se não existir
if (!fs.existsSync(CONFIG)) {
  fs.writeFileSync(CONFIG, JSON.stringify({
    bypass: false,
    aim_assist: false,
    precision_disfarcado: false,
    sensi_alta: false,
    hs_pescoco: false,
    back_jump: false
  }, null, 2));
}

// ==========================================
// 🔒 AQUI FICA SEU CÓDIGO — PROTEGIDO!
// ==========================================
const CODIGOS_ATIVACAO = {

  bypass: `
    // BYPASS • Proteção antiban
    function AtivarBypass() {
      print("✅ BYPASS ATIVADO");
      // COLA SEU CÓDIGO REAL AQUI ↓
      
    }
    AtivarBypass();
  `,

  aim_silent: `
    // AIM SILENT SYSTEM • Mira silenciosa
    function AtivarAimSilent() {
      print("✅ AIM SILENT ATIVADO");
      // COLA SEU CÓDIGO REAL AQUI ↓
      
    }
    AtivarAimAssist();
  `,

  precision_disfarcado: `
    // 🔮 PRECISION DISFARÇADO • Pega sem parecer
    function PrecisionDisfarcado() {
      const forca = 0.35;
      const margem_erro = 0.08;
      
      let alvo = AcharInimigoMaisPerto();
      if (!alvo) return;
      
      let alvoX = alvo.x;
      let alvoY = alvo.cabecaY - 12;
      
      alvoX += (Math.random() - 0.5) * margem_erro;
      alvoY += (Math.random() - 0.5) * margem_erro;
      
      MoverMiraSuave(alvoX, alvoY, forca);
      
      // COLA O RESTO DO SEU CÓDIGO AQUI ↓
      
    }
    PrecisionDisfarcado();
  `,

  sensi_alta: `
    // SENSI ALTA • Sensibilidade 9x
    function AtivarSensiAlta() {
      print("✅ SENSI ALTA ATIVADA");
      // COLA SEU CÓDIGO REAL AQUI ↓
      
    }
    AtivarSensiAlta();
  `,

  hs_pescoco: `
    // HS PESCOÇO • Trava no pescoço
    function AtivarHSPescoco() {
      print("✅ HS PESCOÇO ATIVADO");
      // COLA SEU CÓDIGO REAL AQUI ↓
      
    }
    AtivarHSPescoco();
  `,

  back_jump: `
    // BACK JUMP • Movimento
    function AtivarBackJump() {
      print("✅ BACK JUMP ATIVADO");
      // COLA SEU CÓDIGO REAL AQUI ↓
      
    }
    AtivarBackJump();
  `
};

// Envia só o que tá LIGADO pro celular
app.get('/codigos-ativos', (req, res) => {
  const cfg = JSON.parse(fs.readFileSync(CONFIG));
  let resposta = `-- ARTHUR PROX • GERADO EM ${new Date().toLocaleString('pt-BR')}\n`;
  resposta += `-- CÓDIGO PROTEGIDO • NÃO COMPARTILHAR\n\n`;
  
  for (const [chave, ativo] of Object.entries(cfg)) {
    if (ativo && CODIGOS_ATIVACAO[chave]) {
      resposta += CODIGOS_ATIVACAO[chave] + '\n';
    }
  }
  
  res.type('text/plain').send(resposta);
});

app.get('/config.json', (req, res) => {
  res.setHeader('Content-Type', 'application/json');
  res.sendFile(CONFIG);
});

app.post('/salvar', (req, res) => {
  fs.writeFileSync(CONFIG, JSON.stringify(req.body, null, 2));
  res.json({ ok: true, mensagem: "✅ Salvo com sucesso!" });
});

app.get('/', (req, res) => res.sendFile(path.join(__dirname, 'index.html')));

app.listen(PORTA, '0.0.0.0', () => console.log('✅ ARTHUR PROX ONLINE!'));
