function doGet(e) {
  try {
    const action = (e && e.parameter && e.parameter.action) ? e.parameter.action : 'get';
    const spreadsheet = SpreadsheetApp.getActiveSpreadsheet();

    if (action === 'getTeams') {
      const sheet = getOrCreateSheet(spreadsheet, 'Equipes');
      const teams = getTeamsFromSheet(sheet);
      return jsonResponse({ ok: true, teams: teams });
    }

    if (action === 'save') {
      const sheet = getOrCreateSheet(spreadsheet, 'Classement');
      const name = String(e.parameter.name || 'Anonyme').substring(0, 30).replace(/[<>]/g, '');
      const score = Math.max(0, Math.min(99999, parseInt(e.parameter.score) || 0));
      const acc = Math.max(0, Math.min(100, parseInt(e.parameter.acc) || 0));
      const team = String(e.parameter.team || '').substring(0, 50).replace(/[<>]/g, '');
      const date = new Date().toLocaleDateString('fr-FR');
      sheet.appendRow([name, score, acc, date, team]);
      return jsonResponse({ ok: true });
    }

    if (action === 'get') {
      const sheet = getOrCreateSheet(spreadsheet, 'Classement');
      const values = sheet.getDataRange().getDisplayValues();
      if (values.length <= 1) {
        return jsonResponse({ ok: true, data: [] });
      }

      const rows = values.slice(1)
        .filter(row => row[0] || row[1] || row[2])
        .map(row => ({
          name: row[0] || '',
          score: Number(row[1] || 0),
          acc: Number(row[2] || 0),
          date: row[3] || '',
          team: row[4] || ''
        }))
        .sort((a, b) => b.score - a.score)
        .slice(0, 50);

      return jsonResponse({ ok: true, data: rows });
    }

    return jsonResponse({ ok: false, error: 'Action inconnue' });
  } catch (err) {
    return jsonResponse({ ok: false, error: err.message });
  }
}

function doPost(e) {
  return doGet(e);
}

function getOrCreateSheet(spreadsheet, name) {
  let sheet = spreadsheet.getSheetByName(name);
  if (!sheet) {
    sheet = spreadsheet.insertSheet(name);
    if (name === 'Equipes') {
      sheet.getRange(1, 1, 1, 1).setValue('Équipe');
      sheet.getRange(1, 1, 1, 1).setFontWeight('bold');
    }
    if (name === 'Classement') {
      sheet.getRange(1, 1, 1, 5).setValues([['Pseudo', 'Score', 'Précision (%)', 'Date', 'Équipe']]);
      sheet.getRange(1, 1, 1, 5).setFontWeight('bold');
    }
  }
  return sheet;
}

function getTeamsFromSheet(sheet) {
  const values = sheet.getDataRange().getDisplayValues();
  const teams = [];
  for (const row of values) {
    const team = String(row[0] || '').trim();
    if (team) {
      teams.push(team);
    }
  }
  return teams;
}

function jsonResponse(obj) {
  return ContentService.createTextOutput(JSON.stringify(obj)).setMimeType(ContentService.MimeType.JSON);
}
