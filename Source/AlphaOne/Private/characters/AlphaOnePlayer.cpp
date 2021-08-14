#include "characters/AlphaOnePlayer.h"
#include "controllers/AlphaOnePlayerController.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "Components/TimelineComponent.h"
#include "Kismet/GameplayStatics.h"


// Sets default values
AAlphaOnePlayer::AAlphaOnePlayer()
{
    // Don't rotate when the controller rotates. Let that just affect the camera.
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	// Create a camera boom (pulls in towards the player if there is a collision)
	CameraArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraArm"));
	CameraArm->SetupAttachment(RootComponent);
	CameraArm->bUsePawnControlRotation = true; // Rotate the arm based on the controller

	// Create a follow camera
	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraArm, USpringArmComponent::SocketName); // Attach the camera to the end of the boom and let the boom adjust to match the controller orientation
	FollowCamera->bUsePawnControlRotation = false; // Camera does not rotate relative to arm

    // Create a timeline component for scheduling events
    CameraResetTimeline = CreateDefaultSubobject<UTimelineComponent>(TEXT("CameraResetTimeline"));
    static ConstructorHelpers::FObjectFinder<UCurveFloat> CameraResetCurveObject(TEXT("CurveFloat'/Game/Curve/CameraResetCurve.CameraResetCurve'"));
    if (CameraResetCurveObject.Succeeded()) {
        CameraResetTimeCurve = CameraResetCurveObject.Object;
    }

    DefaultFaction = EUnitFactions::Protagonist;
}

void AAlphaOnePlayer::BeginPlay()
{
    Super::BeginPlay();
    CameraArm->TargetArmLength = DefaultArmLength; // The camera follows at this distance behind the character
    if (CameraResetTimeCurve) {
        CameraResetTimeline->SetTimelineLength(CameraResetTime);
        CameraResetTimeline->SetLooping(false);
        FOnTimelineFloat CameraResetProgress;
        CameraResetProgress.BindUFunction(this, "CameraResetTick");
        CameraResetTimeline->AddInterpFloat(CameraResetTimeCurve, CameraResetProgress);
        FOnTimelineEvent CameraResetComplete;
        CameraResetComplete.BindUFunction(this, "OnCameraResetEnd");
        CameraResetTimeline->SetTimelineFinishedFunc(CameraResetComplete);
    }
}

// NOTE: action needs to be reset by animations
void AAlphaOnePlayer::Jump()
{
	if (!CheckAction(EUnitActions::Jumping)) {
		// GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, TEXT("JUMP!"));
		Super::Jump();
		SetAction(EUnitActions::Jumping);
	}
}

bool AAlphaOnePlayer::ResetCamera()
{
    // GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, TEXT("Reset Camera"));
    auto PlayerController = Cast<AAlphaOnePlayerController>(GetController());
    if (CameraResetTimeline->IsPlaying() || !PlayerController) {
        return false;
    }

    auto CurrentRotation = PlayerController->GetControlRotation();
    auto TargetRotation = GetActorRotation();
    // no need to reset
    if ((CurrentRotation - TargetRotation).IsNearlyZero(0.1f)) {
        return true;
    }
    // GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, TEXT("Reset Camera Play"));
    CameraResetTimeline->PlayFromStart();
    return true;
}

void AAlphaOnePlayer::CameraResetTick_Impl(float InDeltaValue)
{
    // GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, FString::Printf(TEXT("Reset Alpha: %.02f!"), InDeltaValue));
    auto PlayerController = Cast<AAlphaOnePlayerController>(GetController());
    if (PlayerController) {
        auto CurrentRotation = PlayerController->GetControlRotation();
        auto TargetRotation = GetActorRotation();
        PlayerController->SetControlRotation(FQuat::Slerp(CurrentRotation.Quaternion(), TargetRotation.Quaternion(), InDeltaValue).Rotator());
    }
}
