#pragma once

void UpdateSpeedhack() {
    if (StartAimbot) {
        void* Simulation = GetSimulationTimer();
     if (Simulation != nullptr) {
   float FixedDeltaTime = GetTimer(Simulation);
            if(!saved) {
                active = FixedDeltaTime * 1.82f;
                desactive = FixedDeltaTime;
                saved = true;
            }
   if (SpeedHack) {
       if (FixedDeltaTime != active) {
                    SetTimer(Simulation, active);
    }
   }
   else {
       if (FixedDeltaTime != desactive) {
                    SetTimer(Simulation, desactive);
    }
   }
  }
 }
}

bool Visible_Check(void *closestEnemy) {
    void *Object = static_cast<void *>(nullptr);
    Vector3 Camera = Transform_INTERNAL_GetPosition(Component_GetTransform(Camera_main()));
    Vector3 Collider = Transform_INTERNAL_GetPosition(Component_GetTransform(GetHeadCollider(closestEnemy)));
    if (!Cristiano_RayVery(Camera, Collider, 12, &Object)) {
        return true;
    } else {
        return false;
    }
}

void* GetClosestEnemy() {
    float shortestDistance = MAX_DISTANCE;
    void* closestEnemy = NULL; 
    void* get_MatchGame = Curent_Match();
    if (!get_MatchGame || !StartAimbot) return NULL;

    void* LocalPlayer = GetLocalPlayer(get_MatchGame);
    if (!LocalPlayer) return NULL;

    auto players = *(monoDictionary<uint8_t*, void**>**)((long)get_MatchGame + ListPlayer);
    if (!players) return NULL;

    for (int u = 0; u < players->getNumValues(); u++) {
        void* Player = players->getValues()[u]; 
        if (!Player) continue;

        if (!get_isLocalTeam(Player) 
        && (ignoreKnockedEnemies || !get_IsDieing(Player))
        && get_isVisible(Player) 
        && get_MaxHP(Player) > 0) 
        {
            Vector3 PlayerPos = getPosition(Player);
            Vector3 LocalPlayerPos = getPosition(LocalPlayer);
            float distance = Vector3::Distance(LocalPlayerPos, PlayerPos);

            Vector3 targetDir = Vector3::Normalized(PlayerPos - LocalPlayerPos);
            float angle = Vector3::Angle(targetDir, GetForward(Component_GetTransform(Camera_main()))) * AIM_ANGLE_MULTIPLIER;

            if (distance < Aimdis && angle <= Fov_Aim) {
                if (angle < shortestDistance) {
                    shortestDistance = angle;
                    closestEnemy = Player;
                }
            }
        }
    }
    return closestEnemy;
}

void GetPointers() {
    if (!StartAimbot) return;
    SetHighFPS(3);

    current_Match = Curent_Match();
    if (!current_Match) {
        inMatch = false;
        return;
    }

    int matchState = *(int*)((uintptr_t)current_Match + m_State);
    if (matchState != 1) {
        inMatch = false;
        return;
    }
    inMatch = true;

    local_player = GetLocalPlayer(current_Match);
    if (!local_player) {
        inMatch = false;
        return;
    }

    local_playerInMatch = *(void**)((uintptr_t)current_Match + m_LocalPlayer);
    if (!local_playerInMatch) {
        inMatch = false;
        return;
    }

    mainCamera = Camera_main();
    if (!mainCamera) {
        inMatch = false;
        return;
    }

    localPlayerAttributes = *(void**)((uintptr_t)local_player + m_PlayerAttributes);
    if (!localPlayerAttributes) return;

    localTransform = Component_GetTransform(local_player);
    if (!localTransform) return;

    playerLocation = CameraMain(local_player);
    playerPosition = getPosition(local_player);
    playerForward = GetForwardAdjusted(Component_GetTransform(mainCamera), 342.5f);

    hitObjInfo = *(void**)((uintptr_t)local_playerInMatch + m_HawkerHitObject);
    if (!hitObjInfo) return;

    ammoBase = *(Vector3*)((uintptr_t)hitObjInfo + m_HawkerStartPos);
    isFiring = get_IsFiring(local_player);
    isScoped = get_IsSighting(local_player);

    closestEnemy = GetClosestEnemy();
    if (closestEnemy) {
        enemyTransform = Component_GetTransform(closestEnemy);
        if (enemyTransform) {
            enemyPosition = Transform_GetPosition(enemyTransform);
            enemyPos = GetHeadPosition(closestEnemy);
            dir = enemyPos - ammoBase;
            distanceToLocalPlayer = Vector3::Distance(getPosition(local_player), enemyPosition);
        } else {
            enemyPosition = Vector3::zero();
        }
    } else {
        enemyTransform = nullptr;
        enemyPosition = Vector3::zero();
    }
}

Vector3 GetAdjustedPosition(void* closestEnemy) {
    Vector3 headPos = GetHeadPosition(closestEnemy);
    if (aimPosition == 1) {
        headPos.y += NECK_OFFSET;
    } else if (aimPosition == 2) {
        Vector3 hipPos = GetHipPosition(closestEnemy);
        headPos = Vector3::Lerp(headPos, hipPos, 0.5f);
    } else if (aimPosition == 3) {
        headPos = GetHipPosition(closestEnemy);
    }
    return headPos;
}

Quaternion GetCameraRotation(void* localPlayer) {
    return GetRotation(Component_GetTransform(Camera_main()));
}

bool IsInFov(Vector3 enemyPos, Vector3 playerPos, float fovLimit) {
    Vector3 m_HeadScreen = WorldToScreenPoint(mainCamera, GetHeadPosition(closestEnemy));
    float screenCenterX = savedScreenWidth / 2;
    float screenCenterY = savedScreenHeight / 2;
    float distanceToCenterX = m_HeadScreen.x - screenCenterX;
    float distanceToCenterY = m_HeadScreen.y - screenCenterY;
    float distanceToCenter = abs(distanceToCenterX) + abs(distanceToCenterY);
    if (distanceToCenter <= fovLimit) {
        return true;
    } else {
        return false;
    }
}

void AimbotLegitVoid() {
    if (StartAimbot && AimbotLegit) {
        if (closestEnemy != NULL && local_player != NULL && current_Match != NULL) {
            void* Current_Collider = *(void**)((uintptr_t)closestEnemy + 0x78);
            void* Head_Collider = Player_GetHeadCollider(closestEnemy);
            if (Current_Collider != Head_Collider) {
                SetAimCollider(closestEnemy, Head_Collider);
            }
        }
    }
}

void Aimbott() {
if (!StartAimbot) return;
if (!AimbotRage) return;

void* currentMatch = Curent_Match();  
if (!currentMatch) return;  

void* localPlayer = GetLocalPlayer(currentMatch);  
if (!localPlayer) return;  

void* closestEnemy = GetClosestEnemy();
if (!closestEnemy) return;  

if (get_IsDieing(closestEnemy) || get_MaxHP(closestEnemy) <= 0)  
    return;  

Vector3 enemyLocation  = GetAdjustedPosition(closestEnemy);  
Vector3 playerLocation = CameraMain(localPlayer);  

Quaternion currentRotation = GetRotation(Component_GetTransform(Camera_main()));  
Quaternion targetRotation  = GetRotationToLocation(enemyLocation, 0.1f, playerLocation);  

Quaternion smoothRotation;  
if (aimSmoothing <= 0) {  
    smoothRotation = targetRotation;  
} else {  
    float smoothFactor = aimSmoothing / 10.0f;  
    if (smoothFactor < 0.0f) smoothFactor = 0.0f;  
    if (smoothFactor > 1.0f) smoothFactor = 1.0f;  
    smoothRotation = Quaternion::Slerp(currentRotation, targetRotation, smoothFactor);  
}  

bool isScopeOn = get_IsSighting(localPlayer);  
bool isFiring  = get_IsFiring(localPlayer);  

bool shouldAim = false;  
if (aimbotTrigger && isFiring)  
    shouldAim = true;  
if (aimbotAim && isScopeOn)  
    shouldAim = true;  

if (shouldAim) {  
    set_aim(localPlayer, smoothRotation);  
}

}

void AimSilentVoid() {
    while (true) {
        if (StartAimbot && AimSilent) {
            if (local_playerInMatch != nullptr && current_Match != nullptr && hitObjInfo != nullptr) {
                if (isFiring) {
                    *(Vector3*)((uintptr_t)hitObjInfo + m_HawkrtEndPos) = dir;
                }
            }
        }
        std::this_thread::sleep_for(std::chrono::microseconds(1));
    }
}

float savedY = 0.0f;
void* lastTargetID = nullptr;
bool alreadySavedY = false;
bool upThreadRunning = false;
bool stopUpThread = false;
void ResetUpPlayerState() {
    savedY = 0.0f;
    alreadySavedY = false;
    lastTargetID = nullptr;
}
void UpPlayerLogic() {
    while (!stopUpThread && StartAimbot) {
        if (!inMatch) {
            ResetUpPlayerState();
            break;
        }
        if (!StartAimbot || !current_Match || !local_player || !closestEnemy || !enemyTransform || !isFiring) {
            ResetUpPlayerState();
            std::this_thread::sleep_for(std::chrono::milliseconds(15));
            continue;
        }
        if (!IsValidPointer(current_Match) || !IsValidPointer(local_player) || !IsValidPointer(closestEnemy) || !IsValidPointer(enemyTransform)) {
            ResetUpPlayerState();
            std::this_thread::sleep_for(std::chrono::milliseconds(15));
            continue;
        }
        bool enemyInFov = IsInFov(enemyPosition, playerLocation, Fov_Aim);
        if (!enemyInFov && UpPlayer) {
            std::this_thread::sleep_for(std::chrono::milliseconds(10));
            continue;
        }
        if (UpPlayer && (distanceToLocalPlayer > 12.0f || !TPPlayer)) {
            if (closestEnemy != lastTargetID || !alreadySavedY) {
                savedY = enemyPosition.y + 1.5f;
                alreadySavedY = true;
                lastTargetID = closestEnemy;
            }
            if (IsValidPointer(enemyTransform)) {
                Transform_INTERNAL_SetPosition(enemyTransform, Vector3(enemyPosition.x, savedY, enemyPosition.z));
            }
        }
        if (TPPlayer && distanceToLocalPlayer < 12.0f) {
            Vector3 newEnemyPos = playerPosition + (playerForward * 1.5f);
            if (IsValidPointer(enemyTransform)) {
                Transform_INTERNAL_SetPosition(enemyTransform, Vector3(newEnemyPos.x, newEnemyPos.y, newEnemyPos.z));
            }
        }
        std::this_thread::sleep_for(std::chrono::milliseconds(1));
    }
    upThreadRunning = false;
    stopUpThread = false;
    ResetUpPlayerState();
}
void ManageUpPlayerThread() {
    if (inMatch && !upThreadRunning) {
        stopUpThread = false;
        upThreadRunning = true;
        std::thread(UpPlayerLogic).detach();
    } else if (!inMatch && upThreadRunning) {
        stopUpThread = true;
    }
}

bool Reset;



bool(*get_ResetGuest)(bool* instance);
bool _get_ResetGuest(bool* instance) {
    if (Reset) {
        return true;
    }
    return get_ResetGuest(instance);
}
