// by Neil Moore

#include "IRadioGardenAPI.h"
#include "RadioGardenHttpRequest.h"
#include "Async/Async.h"
#include "Dom/JsonObject.h"
#include "Dom/JsonValue.h"

// ========== Places (Места) ==========

void IRadioGardenAPI::GetPlaces(FRadioGardenPlacesResponse& OutResponse)
{
    OutResponse = FRadioGardenPlacesResponse();

    FString ResponseContent;
    FString ErrorMessage;

    if (!FRadioGardenHttpRequest::ExecuteGet(TEXT("/ara/content/places"), ResponseContent, ErrorMessage))
    {
        OutResponse.Status = ERadioGardenStatus::NetworkError;
        OutResponse.ErrorMessage = ErrorMessage;
        OutResponse.bSuccessful = false;
        return;
    }

    TSharedPtr<FJsonObject> JsonObject;
    if (!FRadioGardenHttpRequest::ParseJson(ResponseContent, JsonObject))
    {
        OutResponse.Status = ERadioGardenStatus::ParseError;
        OutResponse.ErrorMessage = TEXT("Failed to parse JSON");
        return;
    }

    // Получаем массив мест
    const TArray<TSharedPtr<FJsonValue>>* PlacesArray;
    if (!FRadioGardenHttpRequest::GetArraySafe(JsonObject, TEXT("data.list"), PlacesArray))
    {
        // Проверяем альтернативный путь
        TSharedPtr<FJsonObject> DataObj;
        if (FRadioGardenHttpRequest::GetObjectSafe(JsonObject, TEXT("data"), DataObj))
        {
            if (!FRadioGardenHttpRequest::GetArraySafe(DataObj, TEXT("list"), PlacesArray))
            {
                OutResponse.Status = ERadioGardenStatus::ParseError;
                OutResponse.ErrorMessage = TEXT("Invalid response format");
                return;
            }
        }
        else
        {
            OutResponse.Status = ERadioGardenStatus::ParseError;
            OutResponse.ErrorMessage = TEXT("Invalid response format");
            return;
        }
    }

    // Парсим места
    for (const TSharedPtr<FJsonValue>& PlaceValue : *PlacesArray)
    {
        const TSharedPtr<FJsonObject>& PlaceObj = PlaceValue->AsObject();
        if (!PlaceObj.IsValid())
        {
            continue;
        }

        FRadioGardenPlace Place;
        Place.Id = FRadioGardenHttpRequest::GetStringSafe(PlaceObj, TEXT("id"));
        Place.Title = FRadioGardenHttpRequest::GetStringSafe(PlaceObj, TEXT("title"));
        Place.Country = FRadioGardenHttpRequest::GetStringSafe(PlaceObj, TEXT("country"));
        Place.Url = FRadioGardenHttpRequest::GetStringSafe(PlaceObj, TEXT("url"));
        Place.Size = static_cast<int32>(FRadioGardenHttpRequest::GetNumberSafe(PlaceObj, TEXT("size")));
        Place.bBoost = FRadioGardenHttpRequest::GetBoolSafe(PlaceObj, TEXT("boost"));

        // Парсим координаты
        const TArray<TSharedPtr<FJsonValue>>* GeoArray;
        if (PlaceObj->TryGetArrayField(TEXT("geo"), GeoArray) && GeoArray->Num() >= 2)
        {
            Place.Geo.Longitude = (*GeoArray)[0]->AsNumber();
            Place.Geo.Latitude = (*GeoArray)[1]->AsNumber();
        }

        OutResponse.Places.Add(Place);
    }

    OutResponse.Status = ERadioGardenStatus::Success;
    OutResponse.bSuccessful = true;
}

void IRadioGardenAPI::GetPlacesAsync(const FOnRadioGardenPlacesReceived& OnCompleted)
{
    AsyncTask(ENamedThreads::AnyBackgroundThreadNormalTask, [OnCompleted]()
    {
        FRadioGardenPlacesResponse Response;
        GetPlaces(Response);

        AsyncTask(ENamedThreads::GameThread, [Response, OnCompleted]()
        {
            OnCompleted.ExecuteIfBound(Response);
        });
    });
}

void IRadioGardenAPI::GetPlaceDetails(const FString& PlaceId, FRadioGardenPlacesResponse& OutResponse)
{
    OutResponse = FRadioGardenPlacesResponse();

    if (!IsValidId(PlaceId))
    {
        OutResponse.Status = ERadioGardenStatus::InvalidResponse;
        OutResponse.ErrorMessage = TEXT("Invalid Place ID");
        return;
    }

    const FString Endpoint = FString::Printf(TEXT("/ara/content/page/%s"), *PlaceId);
    FString ResponseContent;
    FString ErrorMessage;

    if (!FRadioGardenHttpRequest::ExecuteGet(Endpoint, ResponseContent, ErrorMessage))
    {
        OutResponse.Status = ERadioGardenStatus::NetworkError;
        OutResponse.ErrorMessage = ErrorMessage;
        return;
    }

    TSharedPtr<FJsonObject> JsonObject;
    if (!FRadioGardenHttpRequest::ParseJson(ResponseContent, JsonObject))
    {
        OutResponse.Status = ERadioGardenStatus::ParseError;
        OutResponse.ErrorMessage = TEXT("Failed to parse JSON");
        return;
    }

    // В реальной реализации здесь нужно парсить детальную информацию о месте
    // Для упрощения возвращаем базовый ответ
    OutResponse.Status = ERadioGardenStatus::Success;
    OutResponse.bSuccessful = true;
}

void IRadioGardenAPI::GetPlaceDetailsAsync(const FString& PlaceId, const FOnRadioGardenPlacesReceived& OnCompleted)
{
    AsyncTask(ENamedThreads::AnyBackgroundThreadNormalTask, [PlaceId, OnCompleted]()
    {
        FRadioGardenPlacesResponse Response;
        GetPlaceDetails(PlaceId, Response);

        AsyncTask(ENamedThreads::GameThread, [Response, OnCompleted]()
        {
            OnCompleted.ExecuteIfBound(Response);
        });
    });
}

void IRadioGardenAPI::GetPlaceChannels(const FString& PlaceId, FRadioGardenChannelsResponse& OutResponse)
{
    OutResponse = FRadioGardenChannelsResponse();
    OutResponse.PlaceId = PlaceId;

    if (!IsValidId(PlaceId))
    {
        OutResponse.Status = ERadioGardenStatus::InvalidResponse;
        OutResponse.ErrorMessage = TEXT("Invalid Place ID");
        return;
    }

    const FString Endpoint = FString::Printf(TEXT("/ara/content/page/%s/channels"), *PlaceId);
    FString ResponseContent;
    FString ErrorMessage;

    if (!FRadioGardenHttpRequest::ExecuteGet(Endpoint, ResponseContent, ErrorMessage))
    {
        OutResponse.Status = ERadioGardenStatus::NetworkError;
        OutResponse.ErrorMessage = ErrorMessage;
        return;
    }

    TSharedPtr<FJsonObject> JsonObject;
    if (!FRadioGardenHttpRequest::ParseJson(ResponseContent, JsonObject))
    {
        OutResponse.Status = ERadioGardenStatus::ParseError;
        OutResponse.ErrorMessage = TEXT("Failed to parse JSON");
        return;
    }

    // Получаем массив каналов
    TSharedPtr<FJsonObject> DataObj;
    if (!FRadioGardenHttpRequest::GetObjectSafe(JsonObject, TEXT("data"), DataObj))
    {
        OutResponse.Status = ERadioGardenStatus::ParseError;
        OutResponse.ErrorMessage = TEXT("Invalid response format");
        return;
    }

    const TArray<TSharedPtr<FJsonValue>>* ContentArray;
    if (!FRadioGardenHttpRequest::GetArraySafe(DataObj, TEXT("content"), ContentArray) || ContentArray->Num() == 0)
    {
        OutResponse.Status = ERadioGardenStatus::ParseError;
        OutResponse.ErrorMessage = TEXT("No channels found");
        return;
    }

    // Получаем массив из первого элемента content
    const TSharedPtr<FJsonObject>& ContentItemObj = (*ContentArray)[0]->AsObject();
    if (!ContentItemObj.IsValid())
    {
        OutResponse.Status = ERadioGardenStatus::ParseError;
        OutResponse.ErrorMessage = TEXT("Invalid content format");
        return;
    }

    const TArray<TSharedPtr<FJsonValue>>* ItemsArray;
    if (!FRadioGardenHttpRequest::GetArraySafe(ContentItemObj, TEXT("items"), ItemsArray))
    {
        OutResponse.Status = ERadioGardenStatus::ParseError;
        OutResponse.ErrorMessage = TEXT("Invalid items format");
        return;
    }

    // Парсим каналы
    for (const TSharedPtr<FJsonValue>& ItemValue : *ItemsArray)
    {
        const TSharedPtr<FJsonObject>& ChannelObj = ItemValue->AsObject();
        if (!ChannelObj.IsValid())
        {
            continue;
        }

        FRadioGardenChannel Channel;

        // Структура: { page: { url: "/listen/station-name/ChannelId", title: "...", ... } }
        TSharedPtr<FJsonObject> PageObj;
        if (!FRadioGardenHttpRequest::GetObjectSafe(ChannelObj, TEXT("page"), PageObj))
        {
            continue;
        }

        Channel.Title = FRadioGardenHttpRequest::GetStringSafe(PageObj, TEXT("title"));
        FString PageUrl = FRadioGardenHttpRequest::GetStringSafe(PageObj, TEXT("url"));
        Channel.Url = PageUrl;

        // Парсим ID из URL (формат: /listen/station-name/ChannelId)
        if (!PageUrl.IsEmpty())
        {
            TArray<FString> Parts;
            PageUrl.ParseIntoArray(Parts, TEXT("/"), true);
            if (Parts.Num() >= 2)
            {
                Channel.Id = Parts[Parts.Num() - 1];
            }
        }

        OutResponse.Channels.Add(Channel);
    }

    OutResponse.Status = ERadioGardenStatus::Success;
    OutResponse.bSuccessful = true;
}

void IRadioGardenAPI::GetPlaceChannelsAsync(const FString& PlaceId, const FOnRadioGardenChannelsReceived& OnCompleted)
{
    AsyncTask(ENamedThreads::AnyBackgroundThreadNormalTask, [PlaceId, OnCompleted]()
    {
        FRadioGardenChannelsResponse Response;
        GetPlaceChannels(PlaceId, Response);

        AsyncTask(ENamedThreads::GameThread, [Response, OnCompleted]()
        {
            OnCompleted.ExecuteIfBound(Response);
        });
    });
}

// ========== Channels (Станции) ==========

void IRadioGardenAPI::GetChannel(const FString& ChannelId, FRadioGardenChannelResponse& OutResponse)
{
    OutResponse = FRadioGardenChannelResponse();

    if (!IsValidId(ChannelId))
    {
        OutResponse.Status = ERadioGardenStatus::InvalidResponse;
        OutResponse.ErrorMessage = TEXT("Invalid Channel ID");
        return;
    }

    const FString Endpoint = FString::Printf(TEXT("/ara/content/channel/%s"), *ChannelId);
    FString ResponseContent;
    FString ErrorMessage;

    if (!FRadioGardenHttpRequest::ExecuteGet(Endpoint, ResponseContent, ErrorMessage))
    {
        OutResponse.Status = ERadioGardenStatus::NetworkError;
        OutResponse.ErrorMessage = ErrorMessage;
        return;
    }

    TSharedPtr<FJsonObject> JsonObject;
    if (!FRadioGardenHttpRequest::ParseJson(ResponseContent, JsonObject))
    {
        OutResponse.Status = ERadioGardenStatus::ParseError;
        OutResponse.ErrorMessage = TEXT("Failed to parse JSON");
        return;
    }

    TSharedPtr<FJsonObject> DataObj;
    if (!FRadioGardenHttpRequest::GetObjectSafe(JsonObject, TEXT("data"), DataObj))
    {
        OutResponse.Status = ERadioGardenStatus::ParseError;
        OutResponse.ErrorMessage = TEXT("Invalid response format");
        return;
    }

    FRadioGardenChannel& Channel = OutResponse.Channel;
    Channel.Id = FRadioGardenHttpRequest::GetStringSafe(DataObj, TEXT("id"));
    Channel.Title = FRadioGardenHttpRequest::GetStringSafe(DataObj, TEXT("title"));
    Channel.Url = FRadioGardenHttpRequest::GetStringSafe(DataObj, TEXT("url"));
    Channel.Website = FRadioGardenHttpRequest::GetStringSafe(DataObj, TEXT("website"));
    Channel.bSecure = FRadioGardenHttpRequest::GetBoolSafe(DataObj, TEXT("secure"));

    // Парсим место
    TSharedPtr<FJsonObject> PlaceObj;
    if (FRadioGardenHttpRequest::GetObjectSafe(DataObj, TEXT("place"), PlaceObj))
    {
        Channel.PlaceId = FRadioGardenHttpRequest::GetStringSafe(PlaceObj, TEXT("id"));
        Channel.PlaceTitle = FRadioGardenHttpRequest::GetStringSafe(PlaceObj, TEXT("title"));
    }

    // Парсим страну
    TSharedPtr<FJsonObject> CountryObj;
    if (FRadioGardenHttpRequest::GetObjectSafe(DataObj, TEXT("country"), CountryObj))
    {
        Channel.CountryId = FRadioGardenHttpRequest::GetStringSafe(CountryObj, TEXT("id"));
        Channel.CountryTitle = FRadioGardenHttpRequest::GetStringSafe(CountryObj, TEXT("title"));
    }

    OutResponse.Status = ERadioGardenStatus::Success;
    OutResponse.bSuccessful = true;
}

void IRadioGardenAPI::GetChannelAsync(const FString& ChannelId, const FOnRadioGardenChannelReceived& OnCompleted)
{
    AsyncTask(ENamedThreads::AnyBackgroundThreadNormalTask, [ChannelId, OnCompleted]()
    {
        FRadioGardenChannelResponse Response;
        GetChannel(ChannelId, Response);

        AsyncTask(ENamedThreads::GameThread, [Response, OnCompleted]()
        {
            OnCompleted.ExecuteIfBound(Response);
        });
    });
}

bool IRadioGardenAPI::GetChannelStreamUrl(const FString& ChannelId, FString& OutStreamUrl, FString& OutErrorMessage)
{
    OutStreamUrl.Empty();

    if (!IsValidId(ChannelId))
    {
        OutErrorMessage = TEXT("Invalid Channel ID");
        return false;
    }

    const FString Endpoint = FString::Printf(TEXT("/ara/content/listen/%s/channel.mp3"), *ChannelId);

    if (!FRadioGardenHttpRequest::ExecuteGetRedirect(Endpoint, OutStreamUrl, OutErrorMessage))
    {
        return false;
    }

    return !OutStreamUrl.IsEmpty();
}

void IRadioGardenAPI::GetChannelStreamUrlAsync(const FString& ChannelId, const FOnRadioGardenStreamUrlReceived& OnCompleted)
{
    AsyncTask(ENamedThreads::AnyBackgroundThreadNormalTask, [ChannelId, OnCompleted]()
    {
        FString StreamUrl;
        FString ErrorMessage;
        const bool bSuccess = GetChannelStreamUrl(ChannelId, StreamUrl, ErrorMessage);

        AsyncTask(ENamedThreads::GameThread, [bSuccess, StreamUrl, OnCompleted]()
        {
            OnCompleted.ExecuteIfBound(bSuccess, StreamUrl);
        });
    });
}

// ========== Search (Поиск) ==========

void IRadioGardenAPI::Search(const FString& Query, FRadioGardenSearchResponse& OutResponse)
{
    OutResponse = FRadioGardenSearchResponse();
    OutResponse.Query = Query;

    if (Query.IsEmpty())
    {
        OutResponse.Status = ERadioGardenStatus::InvalidResponse;
        OutResponse.ErrorMessage = TEXT("Empty search query");
        return;
    }

    const FString EncodedQuery = Query.Replace(TEXT(" "), TEXT("+")).Replace(TEXT("%20"), TEXT("+"));
    const FString Endpoint = FString::Printf(TEXT("/search?q=%s"), *EncodedQuery);
    FString ResponseContent;
    FString ErrorMessage;

    if (!FRadioGardenHttpRequest::ExecuteGet(Endpoint, ResponseContent, ErrorMessage))
    {
        OutResponse.Status = ERadioGardenStatus::NetworkError;
        OutResponse.ErrorMessage = ErrorMessage;
        return;
    }

    TSharedPtr<FJsonObject> JsonObject;
    if (!FRadioGardenHttpRequest::ParseJson(ResponseContent, JsonObject))
    {
        OutResponse.Status = ERadioGardenStatus::ParseError;
        OutResponse.ErrorMessage = TEXT("Failed to parse JSON");
        return;
    }

    OutResponse.TimeTaken = static_cast<int32>(FRadioGardenHttpRequest::GetNumberSafe(JsonObject, TEXT("took")));

    // Получаем результаты
    TSharedPtr<FJsonObject> HitsObj;
    if (!FRadioGardenHttpRequest::GetObjectSafe(JsonObject, TEXT("hits"), HitsObj))
    {
        OutResponse.Status = ERadioGardenStatus::ParseError;
        OutResponse.ErrorMessage = TEXT("Invalid response format");
        return;
    }

    const TArray<TSharedPtr<FJsonValue>>* HitsArray;
    if (!FRadioGardenHttpRequest::GetArraySafe(HitsObj, TEXT("hits"), HitsArray))
    {
        OutResponse.Status = ERadioGardenStatus::Success;
        OutResponse.bSuccessful = true;
        return;
    }

    // Парсим результаты
    for (const TSharedPtr<FJsonValue>& HitValue : *HitsArray)
    {
        const TSharedPtr<FJsonObject>& HitObj = HitValue->AsObject();
        if (!HitObj.IsValid())
        {
            continue;
        }

        TSharedPtr<FJsonObject> SourceObj;
        if (!FRadioGardenHttpRequest::GetObjectSafe(HitObj, TEXT("_source"), SourceObj))
        {
            continue;
        }

        FRadioGardenSearchResult Result;
        Result.Id = FRadioGardenHttpRequest::GetStringSafe(HitObj, TEXT("_id"));
        Result.Score = static_cast<float>(FRadioGardenHttpRequest::GetNumberSafe(HitObj, TEXT("_score")));
        Result.Type = FRadioGardenHttpRequest::GetStringSafe(SourceObj, TEXT("type"));
        Result.Title = FRadioGardenHttpRequest::GetStringSafe(SourceObj, TEXT("title"));
        Result.Subtitle = FRadioGardenHttpRequest::GetStringSafe(SourceObj, TEXT("subtitle"));
        Result.CountryCode = FRadioGardenHttpRequest::GetStringSafe(SourceObj, TEXT("code"));
        Result.Url = FRadioGardenHttpRequest::GetStringSafe(SourceObj, TEXT("url"));

        OutResponse.Results.Add(Result);
    }

    OutResponse.Status = ERadioGardenStatus::Success;
    OutResponse.bSuccessful = true;
}

void IRadioGardenAPI::SearchAsync(const FString& Query, const FOnRadioGardenSearchCompleted& OnCompleted)
{
    AsyncTask(ENamedThreads::AnyBackgroundThreadNormalTask, [Query, OnCompleted]()
    {
        FRadioGardenSearchResponse Response;
        Search(Query, Response);

        AsyncTask(ENamedThreads::GameThread, [Response, OnCompleted]()
        {
            OnCompleted.ExecuteIfBound(Response);
        });
    });
}

// ========== Geo (Геолокация) ==========

void IRadioGardenAPI::GetGeolocation(FRadioGardenGeolocationResponse& OutResponse)
{
    OutResponse = FRadioGardenGeolocationResponse();

    FString ResponseContent;
    FString ErrorMessage;

    if (!FRadioGardenHttpRequest::ExecuteGet(TEXT("/geo"), ResponseContent, ErrorMessage))
    {
        OutResponse.Status = ERadioGardenStatus::NetworkError;
        OutResponse.ErrorMessage = ErrorMessage;
        return;
    }

    TSharedPtr<FJsonObject> JsonObject;
    if (!FRadioGardenHttpRequest::ParseJson(ResponseContent, JsonObject))
    {
        OutResponse.Status = ERadioGardenStatus::ParseError;
        OutResponse.ErrorMessage = TEXT("Failed to parse JSON");
        return;
    }

    FRadioGardenGeolocation& Geo = OutResponse.Geolocation;
    Geo.Ip = FRadioGardenHttpRequest::GetStringSafe(JsonObject, TEXT("ip"));
    Geo.CountryCode = FRadioGardenHttpRequest::GetStringSafe(JsonObject, TEXT("country_code"));
    Geo.CountryName = FRadioGardenHttpRequest::GetStringSafe(JsonObject, TEXT("country_name"));
    Geo.RegionCode = FRadioGardenHttpRequest::GetStringSafe(JsonObject, TEXT("region_code"));
    Geo.RegionName = FRadioGardenHttpRequest::GetStringSafe(JsonObject, TEXT("region_name"));
    Geo.City = FRadioGardenHttpRequest::GetStringSafe(JsonObject, TEXT("city"));
    Geo.ZipCode = FRadioGardenHttpRequest::GetStringSafe(JsonObject, TEXT("zip_code"));
    Geo.TimeZone = FRadioGardenHttpRequest::GetStringSafe(JsonObject, TEXT("time_zone"));
    Geo.Latitude = FRadioGardenHttpRequest::GetNumberSafe(JsonObject, TEXT("latitude"));
    Geo.Longitude = FRadioGardenHttpRequest::GetNumberSafe(JsonObject, TEXT("longitude"));
    Geo.MetroCode = static_cast<int32>(FRadioGardenHttpRequest::GetNumberSafe(JsonObject, TEXT("metro_code")));

    OutResponse.Status = ERadioGardenStatus::Success;
    OutResponse.bSuccessful = true;
}

void IRadioGardenAPI::GetGeolocationAsync(const FOnRadioGardenGeolocationReceived& OnCompleted)
{
    AsyncTask(ENamedThreads::AnyBackgroundThreadNormalTask, [OnCompleted]()
    {
        FRadioGardenGeolocationResponse Response;
        GetGeolocation(Response);

        AsyncTask(ENamedThreads::GameThread, [Response, OnCompleted]()
        {
            OnCompleted.ExecuteIfBound(Response);
        });
    });
}

// ========== Nearby Channels ==========

namespace
{
    // Вспомогательная структура для хранения места с расстоянием
    struct FPlaceWithDistance
    {
        FRadioGardenPlace Place;
        double Distance;

        FPlaceWithDistance() : Distance(0.0) {}
        FPlaceWithDistance(const FRadioGardenPlace& InPlace, double InDistance)
            : Place(InPlace), Distance(InDistance) {}

        bool operator<(const FPlaceWithDistance& Other) const
        {
            return Distance < Other.Distance;
        }
    };

    // Вычисление расстояния по формуле Хаверсина (в километрах)
    double CalculateDistance(double Lat1, double Lon1, double Lat2, double Lon2)
    {
        const double R = 6371.0; // Радиус Земли в км
        const double DLat = FMath::DegreesToRadians(Lat2 - Lat1);
        const double DLon = FMath::DegreesToRadians(Lon2 - Lon1);

        const double A = FMath::Sin(DLat / 2) * FMath::Sin(DLat / 2) +
                         FMath::Cos(FMath::DegreesToRadians(Lat1)) * FMath::Cos(FMath::DegreesToRadians(Lat2)) *
                         FMath::Sin(DLon / 2) * FMath::Sin(DLon / 2);

        const double C = 2 * FMath::Atan2(FMath::Sqrt(A), FMath::Sqrt(1 - A));
        return R * C;
    }
}

void IRadioGardenAPI::GetNearbyChannelsAsync(double Latitude, double Longitude, int32 ChannelsCount, const FOnRadioGardenNearbyChannelsReceived& OnCompleted)
{
    AsyncTask(ENamedThreads::AnyBackgroundThreadNormalTask, [Latitude, Longitude, ChannelsCount, OnCompleted]()
    {
        FRadioGardenNearbyChannelsResponse Response;
        Response.Status = ERadioGardenStatus::UnknownError;
        Response.bSuccessful = false;

        if (ChannelsCount <= 0)
        {
            Response.Status = ERadioGardenStatus::InvalidResponse;
            Response.ErrorMessage = TEXT("Channels count must be positive");
            AsyncTask(ENamedThreads::GameThread, [Response, OnCompleted]()
            {
                OnCompleted.ExecuteIfBound(Response);
            });
            return;
        }

        // Шаг 1: Получаем все места
        FRadioGardenPlacesResponse PlacesResponse;
        GetPlaces(PlacesResponse);

        if (!PlacesResponse.bSuccessful)
        {
            Response.Status = PlacesResponse.Status;
            Response.ErrorMessage = PlacesResponse.ErrorMessage;
            AsyncTask(ENamedThreads::GameThread, [Response, OnCompleted]()
            {
                OnCompleted.ExecuteIfBound(Response);
            });
            return;
        }

        // Шаг 2: Вычисляем расстояние до каждого места и сортируем
        TArray<FPlaceWithDistance> PlacesWithDistance;
        for (const FRadioGardenPlace& Place : PlacesResponse.Places)
        {
            const double Distance = CalculateDistance(Latitude, Longitude, Place.Geo.Latitude, Place.Geo.Longitude);
            PlacesWithDistance.Add(FPlaceWithDistance(Place, Distance));
        }

        PlacesWithDistance.Sort();

        // Шаг 3: Собираем каналы, начиная с ближайших мест
        TArray<FRadioGardenChannelWithDistance> AllChannels;
        int32 ChannelsNeeded = ChannelsCount;

        for (const FPlaceWithDistance& PlaceWithDist : PlacesWithDistance)
        {
            if (ChannelsNeeded <= 0)
            {
                break;
            }

            FRadioGardenChannelsResponse ChannelsResponse;
            GetPlaceChannels(PlaceWithDist.Place.Id, ChannelsResponse);

            if (ChannelsResponse.bSuccessful)
            {
                const FString BaseUrl = GetBaseUrl();
                for (const FRadioGardenChannel& Channel : ChannelsResponse.Channels)
                {
                    FRadioGardenChannelWithDistance ChannelWithDist;
                    ChannelWithDist.Title = Channel.Title;
                    ChannelWithDist.Distance = PlaceWithDist.Distance;

                    // Формируем URL потока: https://radio.garden/api/ara/content/listen/ChannelId/channel.mp3
                    if (!Channel.Id.IsEmpty())
                    {
                        ChannelWithDist.Url = FString::Printf(TEXT("%s/ara/content/listen/%s/channel.mp3"), *BaseUrl, *Channel.Id);
                    }

                    AllChannels.Add(ChannelWithDist);
                }

                ChannelsNeeded -= ChannelsResponse.Channels.Num();
            }
        }

        if (AllChannels.Num() == 0)
        {
            Response.Status = ERadioGardenStatus::InvalidResponse;
            Response.ErrorMessage = TEXT("No channels found");
            AsyncTask(ENamedThreads::GameThread, [Response, OnCompleted]()
            {
                OnCompleted.ExecuteIfBound(Response);
            });
            return;
        }

        // Шаг 4: Сортируем каналы по расстоянию и ограничиваем количество
        AllChannels.Sort([](const FRadioGardenChannelWithDistance& A, const FRadioGardenChannelWithDistance& B)
        {
            return A.Distance < B.Distance;
        });

        // Берем только нужное количество
        if (AllChannels.Num() > ChannelsCount)
        {
            AllChannels.SetNum(ChannelsCount);
        }

        Response.Channels = AllChannels;
        Response.Status = ERadioGardenStatus::Success;
        Response.bSuccessful = true;

        AsyncTask(ENamedThreads::GameThread, [Response, OnCompleted]()
        {
            OnCompleted.ExecuteIfBound(Response);
        });
    });
}

void IRadioGardenAPI::GetNearbyChannelsByGeolocationAsync(int32 ChannelsCount, const FOnRadioGardenNearbyChannelsReceived& OnCompleted)
{
    // Сначала получаем геолокацию, затем вызываем GetNearbyChannelsAsync
    AsyncTask(ENamedThreads::AnyBackgroundThreadNormalTask, [ChannelsCount, OnCompleted]()
    {
        FRadioGardenGeolocationResponse GeoResponse;
        GetGeolocation(GeoResponse);

        if (!GeoResponse.bSuccessful)
        {
            FRadioGardenNearbyChannelsResponse Response;
            Response.Status = GeoResponse.Status;
            Response.ErrorMessage = GeoResponse.ErrorMessage;
            Response.bSuccessful = false;

            AsyncTask(ENamedThreads::GameThread, [Response, OnCompleted]()
            {
                OnCompleted.ExecuteIfBound(Response);
            });
            return;
        }

        // Используем координаты из геолокации
        const double Latitude = GeoResponse.Geolocation.Latitude;
        const double Longitude = GeoResponse.Geolocation.Longitude;

        // Вызываем GetNearbyChannelsAsync с полученными координатами
        // Внимание: этот вызов создаст еще один AsyncTask, что нормально
        GetNearbyChannelsAsync(Latitude, Longitude, ChannelsCount, OnCompleted);
    });
}

// ========== Utility ==========

bool IRadioGardenAPI::IsValidId(const FString& Id)
{
    return !Id.IsEmpty() && Id.Len() <= 50;
}

FString IRadioGardenAPI::GetBaseUrl()
{
    return FRadioGardenHttpRequest::BaseUrl;
}
