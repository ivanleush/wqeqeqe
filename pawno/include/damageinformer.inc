enum E_DAMAGE_FEED_HIT {
    e_Issuer,
    e_Name[MAX_PLAYER_NAME],
    Float:e_Amount,
    e_Weapon,
    e_Tick
}
new PlayerText:s_DamageFeedTaken[MAX_PLAYERS] = {PlayerText:INVALID_TEXT_DRAW, ...};
new PlayerText:s_DamageFeedGiven[MAX_PLAYERS] = {PlayerText:INVALID_TEXT_DRAW, ...};
new s_DamageFeedHitsGiven[MAX_PLAYERS][5][E_DAMAGE_FEED_HIT];
new s_DamageFeedHitsTaken[MAX_PLAYERS][5][E_DAMAGE_FEED_HIT];
new s_DamageFeedTimer[MAX_PLAYERS];
new s_DamageFeedLastUpdate[MAX_PLAYERS];

public: WC_DamageFeedUpdate(playerid)
{
	s_DamageFeedTimer[playerid] = 0;

	if (IsPlayerConnected(playerid) && SettingData[playerid][sdDamageInformer]) {
		DamageFeedUpdate(playerid, true);
	}
}

stock DamageFeedUpdate(playerid, bool:modified = false)
{
	new tick = GetTickCount();
	if (tick == 0) tick = 1;
	new lowest_tick = tick + 1;

	for (new i = 0, j = 0; i < sizeof(s_DamageFeedHitsGiven[]) - 1; i++) {
		if (!s_DamageFeedHitsGiven[playerid][i][e_Tick]) {
			break;
		}

		if (tick - s_DamageFeedHitsGiven[playerid][i][e_Tick] >= 3000) {
			modified = true;

			for (j = i; j < sizeof(s_DamageFeedHitsGiven[]) - 1; j++) {
				s_DamageFeedHitsGiven[playerid][j][e_Tick] = 0;
			}

			break;
		}

		if (s_DamageFeedHitsGiven[playerid][i][e_Tick] < lowest_tick) {
			lowest_tick = s_DamageFeedHitsGiven[playerid][i][e_Tick];
		}
	}

	for (new i = 0, j = 0; i < sizeof(s_DamageFeedHitsTaken[]) - 1; i++) {
		if (!s_DamageFeedHitsTaken[playerid][i][e_Tick]) {
			break;
		}

		if (tick - s_DamageFeedHitsTaken[playerid][i][e_Tick] >= 3000) {
			modified = true;

			for (j = i; j < sizeof(s_DamageFeedHitsTaken[]) - 1; j++) {
				s_DamageFeedHitsTaken[playerid][j][e_Tick] = 0;
			}

			break;
		}

		if (s_DamageFeedHitsTaken[playerid][i][e_Tick] < lowest_tick) {
			lowest_tick = s_DamageFeedHitsTaken[playerid][i][e_Tick];
		}
	}

	if (s_DamageFeedTimer[playerid]) {
		KillTimer(s_DamageFeedTimer[playerid]);
	}

	if (tick - s_DamageFeedLastUpdate[playerid] < 250 && modified) {
		s_DamageFeedTimer[playerid] = SetTimerEx("WC_DamageFeedUpdate", 250 - (tick - s_DamageFeedLastUpdate[playerid]), false, "i", playerid);
	} else {
		if (lowest_tick == tick + 1) {
			s_DamageFeedTimer[playerid] = 0;
			modified = true;
		} else {
			s_DamageFeedTimer[playerid] = SetTimerEx("WC_DamageFeedUpdate", 3000 - (tick - lowest_tick) + 10, false, "i", playerid);
		}

		if (modified) {
			DamageFeedUpdateText(playerid);

			s_DamageFeedLastUpdate[playerid] = tick;
		}
	}
}

stock DamageFeedUpdateText(playerid)
{
	new buf[64 * 5] = "";

	for (new i = 0, weapon[32]; i < sizeof(s_DamageFeedHitsGiven[]) - 1; i++) {
		if (!s_DamageFeedHitsGiven[playerid][i][e_Tick]) {
			break;
		}

		if (_:s_DamageFeedHitsGiven[playerid][i][e_Weapon] == -1) {
			weapon = "Multiple";
		} else {
			GetWeaponName(s_DamageFeedHitsGiven[playerid][i][e_Weapon], weapon, 32);
		}

		if (s_DamageFeedHitsGiven[playerid][i][e_Issuer] == INVALID_PLAYER_ID) {
			format(
				buf,
				sizeof(buf),
				"%s%s +%.2f~n~",
				buf,
				weapon,
				s_DamageFeedHitsGiven[playerid][i][e_Amount] + 0.009
			);
		} else {
			format(
				buf,
				sizeof(buf),
				"%s%s - %s(%d) +%.2f (%.2f)~n~",
				buf,
				s_DamageFeedHitsGiven[playerid][i][e_Name],
				weapon,
                s_DamageFeedHitsGiven[playerid][i][e_Weapon],
				s_DamageFeedHitsGiven[playerid][i][e_Amount] + 0.009,
				PI[s_DamageFeedHitsGiven[playerid][i][e_Issuer]][pHealth]
			);
		}
	}

	if (buf[0]) {
        PlayerTextDrawSetString(playerid, s_DamageFeedGiven[playerid], buf);
        PlayerTextDrawShow(playerid, s_DamageFeedGiven[playerid]);
    } else {
        PlayerTextDrawHide(playerid, s_DamageFeedGiven[playerid]);
    }

	buf = "";

	for (new i = 0, weapon[32]; i < sizeof(s_DamageFeedHitsTaken[]) - 1; i++) {
		if (!s_DamageFeedHitsTaken[playerid][i][e_Tick]) {
			break;
		}

		if (_:s_DamageFeedHitsTaken[playerid][i][e_Weapon] == -1) {
			weapon = "Multiple";
		} else {
			GetWeaponName(s_DamageFeedHitsTaken[playerid][i][e_Weapon], weapon, 32);
		}

		if (s_DamageFeedHitsTaken[playerid][i][e_Issuer] == INVALID_PLAYER_ID) {
			format(
				buf,
				sizeof(buf),
				"%s%s -%.2f (%.2f)~n~",
				buf,
				weapon,
				s_DamageFeedHitsTaken[playerid][i][e_Amount] + 0.009,
                PI[playerid][pHealth]
			);
		} else {
			format(
				buf,
				sizeof(buf),
				"%s%s - %s(%d) -%.2f (%.2f)~n~",
				buf,
				s_DamageFeedHitsTaken[playerid][i][e_Name],
				weapon,
                s_DamageFeedHitsTaken[playerid][i][e_Weapon],
				s_DamageFeedHitsTaken[playerid][i][e_Amount] + 0.009,
                PI[s_DamageFeedHitsGiven[playerid][i][e_Issuer]][pHealth]
			);
		}
	}

    if (buf[0]) {
        PlayerTextDrawSetString(playerid, s_DamageFeedTaken[playerid], buf);
        PlayerTextDrawShow(playerid, s_DamageFeedTaken[playerid]);
    } else {
        PlayerTextDrawHide(playerid, s_DamageFeedTaken[playerid]);
    }
}

stock DamageFeedAddHitGiven(playerid, issuerid, Float:amount, weapon) DamageFeedAddHit(s_DamageFeedHitsGiven[playerid], playerid, issuerid, amount, weapon);
stock DamageFeedAddHitTaken(playerid, issuerid, Float:amount, weapon) DamageFeedAddHit(s_DamageFeedHitsTaken[playerid], playerid, issuerid, amount, weapon);

stock DamageFeedAddHit(arr[5][E_DAMAGE_FEED_HIT], playerid, issuerid, Float:amount, weapon)
{
	if (!SettingData[playerid][sdDamageInformer]) {
		return;
	}

	new tick = GetTickCount();
	if (tick == 0) tick = 1;
	new idx = -1;

	for (new i = 0; i < sizeof(arr) - 1; i++) {
		if (!arr[i][e_Tick]) {
			break;
		}

		if (tick - arr[i][e_Tick] >= 3000) {
			DamageFeedRemoveHit(arr, i);
			break;
		}

		if (arr[i][e_Issuer] == issuerid) {
			if (arr[i][e_Weapon] != weapon) {
				//weapon = -1;
			}

			amount += arr[i][e_Amount];
			idx = i;
			break;
		}
	}

	if (idx == -1) {
		idx = 0;

		for (new i = sizeof(arr) - 1; i >= 1; i--) {
			arr[i] = arr[i - 1];
		}
	}

	arr[idx][e_Tick] = tick;
	arr[idx][e_Amount] = amount;
	arr[idx][e_Issuer] = issuerid;
	arr[idx][e_Weapon] = weapon;

	GetPlayerName(issuerid, arr[idx][e_Name], MAX_PLAYER_NAME);

	DamageFeedUpdate(playerid, true);
}

stock DamageFeedRemoveHit(arr[5][E_DAMAGE_FEED_HIT], idx)
{
	for (new i = 0; i < 5; i++) {
		if (i >= idx) {
			arr[i][e_Tick] = 0;
		}
	}
}