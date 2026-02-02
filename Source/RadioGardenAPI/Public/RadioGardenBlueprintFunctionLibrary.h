// by Neil Moore

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "RadioGardenTypes.h"
#include "RadioGardenBlueprintFunctionLibrary.generated.h"

/**
 * Blueprint Function Library для Radio Garden API
 * Предоставляет удобный доступ к API из Blueprints
 */
UCLASS()
class URadioGardenBlueprintFunctionLibrary : public UBlueprintFunctionLibrary
{
    GENERATED_BODY()

public:
    // ========== Places (Места) ==========

    /**
     * Получить список всех мест с радиостанциями (асинхронно)
     * @param OnCompleted Делегат завершения
     */
    UFUNCTION(BlueprintCallable, Category = "Radio Garden API")
    static void GetPlaces(const FOnRadioGardenPlacesReceived& OnCompleted);

    /**
     * Получить детальную информацию о месте (асинхронно)
     * @param PlaceId ID места
     * @param OnCompleted Делегат завершения
     */
    UFUNCTION(BlueprintCallable, Category = "Radio Garden API")
    static void GetPlaceDetails(const FString& PlaceId, const FOnRadioGardenPlacesReceived& OnCompleted);

    /**
     * Получить все станции в месте (асинхронно)
     * @param PlaceId ID места
     * @param OnCompleted Делегат завершения
     */
    UFUNCTION(BlueprintCallable, Category = "Radio Garden API")
    static void GetPlaceChannels(const FString& PlaceId, const FOnRadioGardenChannelsReceived& OnCompleted);

    // ========== Channels (Станции) ==========

    /**
     * Получить информацию о станции (асинхронно)
     * @param ChannelId ID станции
     * @param OnCompleted Делегат завершения
     */
    UFUNCTION(BlueprintCallable, Category = "Radio Garden API")
    static void GetChannel(const FString& ChannelId, const FOnRadioGardenChannelReceived& OnCompleted);

    /**
     * Получить прямую ссылку на поток станции (асинхронно)
     * @param ChannelId ID станции
     * @param OnCompleted Делегат завершения (bSuccess, StreamUrl)
     */
    UFUNCTION(BlueprintCallable, Category = "Radio Garden API")
    static void GetChannelStreamUrl(const FString& ChannelId, const FOnRadioGardenStreamUrlReceived& OnCompleted);

    // ========== Search (Поиск) ==========

    /**
     * Поиск станций, мест и стран (асинхронно)
     * @param Query Поисковый запрос
     * @param OnCompleted Делегат завершения
     */
    UFUNCTION(BlueprintCallable, Category = "Radio Garden API")
    static void Search(const FString& Query, const FOnRadioGardenSearchCompleted& OnCompleted);

    // ========== Geo (Геолокация) ==========

    /**
     * Получить геолокацию клиента (асинхронно)
     * @param OnCompleted Делегат завершения
     */
    UFUNCTION(BlueprintCallable, Category = "Radio Garden API")
    static void GetGeolocation(const FOnRadioGardenGeolocationReceived& OnCompleted);

    // ========== Utility ==========

    /**
     * Проверить валидность ответа
     * @param Response Ответ API
     * @return true если запрос успешен
     */
    UFUNCTION(BlueprintPure, Category = "Radio Garden API")
    static bool IsResponseSuccessful(const FRadioGardenApiResponse& Response);

    /**
     * Получить сообщение об ошибке из ответа
     * @param Response Ответ API
     * @return Текст ошибки
     */
    UFUNCTION(BlueprintPure, Category = "Radio Garden API")
    static FString GetErrorMessage(const FRadioGardenApiResponse& Response);

    /**
     * Получить базовый URL API
     * @return Базовый URL
     */
    UFUNCTION(BlueprintPure, Category = "Radio Garden API")
    static FString GetAPIBaseUrl();

    /**
     * Конвертировать координаты в строку
     * @param Coords Координаты
     * @return Строка в формате "lat,lon"
     */
    UFUNCTION(BlueprintPure, Category = "Radio Garden API")
    static FString CoordsToString(const FRadioGardenCoords& Coords);
};
