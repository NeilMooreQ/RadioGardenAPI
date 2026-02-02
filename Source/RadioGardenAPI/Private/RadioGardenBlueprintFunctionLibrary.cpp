// by Neil Moore

#include "RadioGardenBlueprintFunctionLibrary.h"
#include "IRadioGardenAPI.h"

// ========== Places (Места) ==========

void URadioGardenBlueprintFunctionLibrary::GetPlaces(const FOnRadioGardenPlacesReceived& OnCompleted)
{
    IRadioGardenAPI::GetPlacesAsync(OnCompleted);
}

void URadioGardenBlueprintFunctionLibrary::GetPlaceDetails(const FString& PlaceId, const FOnRadioGardenPlacesReceived& OnCompleted)
{
    IRadioGardenAPI::GetPlaceDetailsAsync(PlaceId, OnCompleted);
}

void URadioGardenBlueprintFunctionLibrary::GetPlaceChannels(const FString& PlaceId, const FOnRadioGardenChannelsReceived& OnCompleted)
{
    IRadioGardenAPI::GetPlaceChannelsAsync(PlaceId, OnCompleted);
}

// ========== Channels (Станции) ==========

void URadioGardenBlueprintFunctionLibrary::GetChannel(const FString& ChannelId, const FOnRadioGardenChannelReceived& OnCompleted)
{
    IRadioGardenAPI::GetChannelAsync(ChannelId, OnCompleted);
}

void URadioGardenBlueprintFunctionLibrary::GetChannelStreamUrl(const FString& ChannelId, const FOnRadioGardenStreamUrlReceived& OnCompleted)
{
    IRadioGardenAPI::GetChannelStreamUrlAsync(ChannelId, OnCompleted);
}

// ========== Search (Поиск) ==========

void URadioGardenBlueprintFunctionLibrary::Search(const FString& Query, const FOnRadioGardenSearchCompleted& OnCompleted)
{
    IRadioGardenAPI::SearchAsync(Query, OnCompleted);
}

// ========== Geo (Геолокация) ==========

void URadioGardenBlueprintFunctionLibrary::GetGeolocation(const FOnRadioGardenGeolocationReceived& OnCompleted)
{
    IRadioGardenAPI::GetGeolocationAsync(OnCompleted);
}

// ========== Utility ==========

bool URadioGardenBlueprintFunctionLibrary::IsResponseSuccessful(const FRadioGardenApiResponse& Response)
{
    return Response.bSuccessful;
}

FString URadioGardenBlueprintFunctionLibrary::GetErrorMessage(const FRadioGardenApiResponse& Response)
{
    return Response.ErrorMessage;
}

FString URadioGardenBlueprintFunctionLibrary::GetAPIBaseUrl()
{
    return IRadioGardenAPI::GetBaseUrl();
}

FString URadioGardenBlueprintFunctionLibrary::CoordsToString(const FRadioGardenCoords& Coords)
{
    return FString::Printf(TEXT("%.6f,%.6f"), Coords.Latitude, Coords.Longitude);
}
