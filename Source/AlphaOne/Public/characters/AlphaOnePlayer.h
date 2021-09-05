// A base player class
// It adds some functions dealing with camera operations, environment interactions, and animation notifies, which are too expensive for an NPC

#pragma once

#include "AlphaOne.h"
#include "characters/CharacterBase.h"
#include "AlphaOnePlayer.generated.h"

UCLASS()
class ALPHAONE_API AAlphaOnePlayer : public ACharacterBase
{
    GENERATED_BODY()

public:
    AAlphaOnePlayer();

    bool ResetCamera();

    FORCEINLINE class USpringArmComponent* GetCameraArm() const { return CameraArm; }

	FORCEINLINE class UCameraComponent* GetFollowCamera() const { return FollowCamera; }

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
    virtual void Tick(float DeltaTime) override;

    UFUNCTION()
    void CameraResetTick(float InDeltaValue) { CameraResetTick_Impl(InDeltaValue); }
    // an overridable function to get around UFUNCTION
    virtual void CameraResetTick_Impl(float InDeltaValue);

    // overridable
    UFUNCTION()
    void OnCameraResetEnd() { OnCameraResetEnd_Impl(); }
    // an overridable function to get around UFUNCTION
    virtual void OnCameraResetEnd_Impl() {}

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera", meta = (AllowPrivateAccess = "true"))
	class USpringArmComponent* CameraArm;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera", meta = (AllowPrivateAccess = "true"))
	class UCameraComponent* FollowCamera;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera", meta = (AllowPrivateAccess = "true"))
    class UTimelineComponent *CameraResetTimeline;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Camera", meta = (AllowPrivateAccess = "true"))
	float DefaultArmLength = 500.f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Camera", meta = (AllowPrivateAccess = "true"))
    UCurveFloat* CameraResetTimeCurve;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Camera", meta = (AllowPrivateAccess = "true"))
    float CameraResetTime = 0.5f;
};
