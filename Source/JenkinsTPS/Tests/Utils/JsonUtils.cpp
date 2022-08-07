// TPS Game For Unreal Automation

#include "Tests/Utils/JsonUtils.h"
#include "Dom/JsonObject.h"
#include "Serialization/JsonWriter.h"
#include "JsonUtilities/Public/JsonObjectConverter.h"
#include "Misc/FileHelper.h"

DEFINE_LOG_CATEGORY_STATIC(LogJsonUtils, All, All);

using namespace TPS::Test;

bool JsonUtils::WriteInputData(const FString& FileName, const FInputData& InputData)
{
    TSharedPtr<FJsonObject> MainJsonObject = FJsonObjectConverter::UStructToJsonObject(InputData);
    if (!MainJsonObject.IsValid()) return false;

    FString OutputString;
    TSharedRef<TJsonWriter<>> JsonWriter = TJsonWriterFactory<>::Create(&OutputString);
    if (!FJsonSerializer::Serialize(MainJsonObject.ToSharedRef(), JsonWriter))
    {
        UE_LOG(LogJsonUtils, Error, TEXT("JSON Serialization Failed"));
        return false;
    }
    if (!JsonWriter->Close())
    {
        UE_LOG(LogJsonUtils, Error, TEXT("JSON Writer Failed"));
        return false;
    }
    if (!FFileHelper::SaveStringToFile(OutputString, *FileName))
    {
        UE_LOG(LogJsonUtils, Error, TEXT("JSON Save To File %s Failed"), *FileName);
        return false;
    }
    return true;
}

bool JsonUtils::ReadInputData(const FString& FileName, FInputData& InputData)
{
    TSharedPtr<FJsonObject> MainJsonObject = MakeShareable(new FJsonObject());
    FString FileContentString;
    if (!FFileHelper::LoadFileToString(FileContentString, *FileName))
    {
        UE_LOG(LogJsonUtils, Error, TEXT("JSON File %s Opening Error"), *FileName);
        return false;
    }
    TSharedRef<TJsonReader<>> JsonReader = TJsonReaderFactory<>::Create(FileContentString);
    if (!FJsonSerializer::Deserialize(JsonReader, MainJsonObject))
    {
        UE_LOG(LogJsonUtils, Error, TEXT("JSON Deserealization Error"));
        return false;
    }
    if (!FJsonObjectConverter::JsonObjectToUStruct(MainJsonObject.ToSharedRef(), &InputData))
    {
        UE_LOG(LogJsonUtils, Error, TEXT("JSON Conversion To UStruct Error"));
        return false;
    }
    return true;
}
