#include "PlatformGameMode.h"
#include "PlayerCharacter.h"
#include "PlayerCharacterController.h"

APlatformGameMode::APlatformGameMode()
{
	DefaultPawnClass = APlayerCharacter::StaticClass();
	PlayerControllerClass = APlayerCharacterController::StaticClass();
}
