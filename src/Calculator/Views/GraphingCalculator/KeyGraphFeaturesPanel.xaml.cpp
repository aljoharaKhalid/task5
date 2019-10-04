#include "pch.h"
#include "KeyGraphFeaturesPanel.xaml.h"
#include "Controls/MathRichEditBox.h"

using namespace CalculatorApp;
using namespace CalculatorApp::ViewModel;
using namespace Platform;
using namespace Platform::Collections;
using namespace Windows::UI;
using namespace Windows::UI::Xaml;
using namespace Windows::UI::Xaml::Controls;

KeyGraphFeaturesPanel::KeyGraphFeaturesPanel()
    : m_Domain{ "" }
    , m_Range{ "" }
    , m_XIntercept{ "" }
    , m_YIntercept{ "" }
    , m_Parity{ "" }
    , m_Periodicity{ "" }
    , m_Minima{ "" }
    , m_Maxima{ "" }
    , m_InflectionPoints{ "" }
    , m_Monotonicity{ "" }
    , m_VerticalAsymptotes{ "" }
    , m_HorizontalAsymptotes{ "" }
    , m_ObliqueAsymptotes{ "" }
    , m_resourceLoader{ Windows::ApplicationModel::Resources::ResourceLoader::GetForCurrentView() }
{
    InitializeComponent();
}
void KeyGraphFeaturesPanel::OnPropertyChanged(String ^ propertyName)
{
    if (propertyName == L"ViewModel")
    {
        if (ViewModel == nullptr)
        {
            return;
        }
        if (ViewModel->AnalysisNotSupported)
        {
            AnalysisNotSupported = m_resourceLoader->GetString(L"KGFAnalysisNotSupported");
            AnalysisNotSupportedTextBlock->Visibility = Windows::UI::Xaml::Visibility::Visible;
            KeyGraphFeaturesStackPanel->Visibility = Windows::UI::Xaml::Visibility::Collapsed;
        }
        else
        {
            AnalysisNotSupportedTextBlock->Visibility = Windows::UI::Xaml::Visibility::Collapsed;
            KeyGraphFeaturesStackPanel->Visibility = Windows::UI::Xaml::Visibility::Visible;

            DomainTextBlock->Text = m_resourceLoader->GetString(L"Domain");
            RangeTextBlock->Text = m_resourceLoader->GetString(L"Range");
            XInterceptTextBlock->Text = m_resourceLoader->GetString(L"XIntercept");
            YInterceptTextBlock->Text = m_resourceLoader->GetString(L"YIntercept");
            ParityTextBlock->Text = m_resourceLoader->GetString(L"Parity");
            PeriodicityTextBlock->Text = m_resourceLoader->GetString(L"Periodicity");
            MinimaTextBlock->Text = m_resourceLoader->GetString(L"Minima");
            MaximaTextBlock->Text = m_resourceLoader->GetString(L"Maxima");
            InflectionPointsTextBlock->Text = m_resourceLoader->GetString(L"InflectionPoints");
            VerticalAsymptotesTextBlock->Text = m_resourceLoader->GetString(L"VerticalAsymptotes");
            HorizontalAsymptotesTextBlock->Text = m_resourceLoader->GetString(L"HorizontalAsymptotes");
            ObliqueAsymptotesTextBlock->Text = m_resourceLoader->GetString(L"ObliqueAsymptotes");
            MonotonicityTextBlock->Text = m_resourceLoader->GetString(L"Monotonicity");

            Domain = ViewModel->Domain;
            Range = ViewModel->Range;
            XIntercept = ViewModel->XIntercept != L"" ? ViewModel->XIntercept : m_resourceLoader->GetString(L"KGFXInterceptNone");
            YIntercept = ViewModel->YIntercept != L"" ? ViewModel->YIntercept : m_resourceLoader->GetString(L"KGFYInterceptNone");
            SetParityStringProperty();
            SetPeriodicityStringProperty();
            Minima = ViewModel->Minima->Size != 0 ? ConvertVectorToString(ViewModel->Minima) : m_resourceLoader->GetString(L"KGFMinimaNone");
            Maxima = ViewModel->Maxima->Size != 0 ? ConvertVectorToString(ViewModel->Maxima) : m_resourceLoader->GetString(L"KGFMaximaNone");
            InflectionPoints = ViewModel->InflectionPoints->Size != 0 ? ConvertVectorToString(ViewModel->InflectionPoints)
                                                                      : m_resourceLoader->GetString(L"KGFInflectionPointsNone");
            VerticalAsymptotes = ViewModel->VerticalAsymptotes->Size != 0 ? ConvertVectorToString(ViewModel->VerticalAsymptotes)
                                                                          : m_resourceLoader->GetString(L"KGFVerticalAsymptotesNone");
            HorizontalAsymptotes = ViewModel->HorizontalAsymptotes->Size != 0 ? ConvertVectorToString(ViewModel->HorizontalAsymptotes)
                                                                              : m_resourceLoader->GetString(L"KGFHorizontalAsymptotesNone");
            ObliqueAsymptotes = ViewModel->ObliqueAsymptotes->Size != 0 ? ConvertVectorToString(ViewModel->ObliqueAsymptotes)
                                                                        : m_resourceLoader->GetString(L"KGFObliqueAsymptotesNone");
            SetMonotoncityStringProperty();
            SetTooComplexFeaturesErrorProperty();
        }
    }
}

void KeyGraphFeaturesPanel::SetParityStringProperty()
{
    if (ViewModel->Parity)
    {
        switch (ViewModel->Parity)
        {
        case 0:
            Parity = m_resourceLoader->GetString(L"KGFParityUnknown");
            break;
        case 1:
            Parity = m_resourceLoader->GetString(L"KGFParityOdd");
            break;
        case 2:
            Parity = m_resourceLoader->GetString(L"KGFParityEven");
            break;
        case 3:
            Parity = m_resourceLoader->GetString(L"KGFParityNeither");
            break;
        default:
            Parity = m_resourceLoader->GetString(L"KGFParityError");
        }
    }
}

void KeyGraphFeaturesPanel::SetPeriodicityStringProperty()
{
    if (ViewModel->Periodicity->Size != 0)
    {
        auto periodicityType = ViewModel->Periodicity->First()->Current->Key->Data();
        switch (*periodicityType)
        {
        case '0':
            Periodicity = m_resourceLoader->GetString(L"KGFPeriodicityUnknown");
            break;
        case '1':
            Periodicity = ViewModel->Periodicity->First()->Current->Value;
            break;
        case '2':
            Periodicity = m_resourceLoader->GetString(L"KGFPeriodicityNotPeriodic");
            break;
        default:
            Periodicity = m_resourceLoader->GetString(L"KGFPeriodicityError");
        }
    }
}

void KeyGraphFeaturesPanel::SetMonotoncityStringProperty()
{
    if (ViewModel->Monotonicity->Size != 0)
    {
        int monotonicitySize = ViewModel->Monotonicity->Size;
        int i = 0;
        for (auto item : ViewModel->Monotonicity)
        {
            Monotonicity += item->Key;
            Monotonicity += " ";

            auto monotonicityType = item->Value->Data();
            switch (*monotonicityType)
            {
            case '0':
                Monotonicity += m_resourceLoader->GetString(L"KGFMonotonicityUnknown");
                break;
            case '1':
                Monotonicity += m_resourceLoader->GetString(L"KGFMonotonicityIncreasing");
                break;
            case '2':
                Monotonicity += m_resourceLoader->GetString(L"KGFMonotonicityDecreasing");
                break;
            case '3':
                Monotonicity += m_resourceLoader->GetString(L"KGFMonotonicityConstant");
                break;
            default:
                Monotonicity += m_resourceLoader->GetString(L"KGFMonotonicityError");
                break;
            }

            if (i < monotonicitySize - 1)
            {
                Monotonicity += "\n";
            }
        }
    }
}

String ^ KeyGraphFeaturesPanel::ConvertVectorToString(Windows::Foundation::Collections::IObservableVector<String ^> ^ inVector)
{
    String ^ outString = ref new String();
    int vectorSize = inVector->Size;
    for (int i = 0; i < vectorSize; i++)
    {
        outString += inVector->GetAt(i);
        if (i < vectorSize - 1)
        {
            outString += "\n";
        }
    }

    return outString;
}

void KeyGraphFeaturesPanel::SetTooComplexFeaturesErrorProperty()
{
    if (ViewModel->TooComplexFeatures == -1)
    {
        return;
    }

    std::wstring error;
    if ((ViewModel->TooComplexFeatures & KeyGraphFeaturesFlag::Domain) == KeyGraphFeaturesFlag::Domain)
    {
        DomainEditbox->Visibility = Windows::UI::Xaml::Visibility::Collapsed;
        DomainTextBlock->Visibility = Windows::UI::Xaml::Visibility::Collapsed;
        error.append((m_resourceLoader->GetString(L"Domain") + L", ")->Data());
    }
    else if ((ViewModel->TooComplexFeatures & KeyGraphFeaturesFlag::Range) == KeyGraphFeaturesFlag::Range)
    {
        RangeEditBox->Visibility = Windows::UI::Xaml::Visibility::Collapsed;
        RangeTextBlock->Visibility = Windows::UI::Xaml::Visibility::Collapsed;
        error.append((m_resourceLoader->GetString(L"Range") + L", ")->Data());
    }
    else if ((ViewModel->TooComplexFeatures & KeyGraphFeaturesFlag::Zeros) == KeyGraphFeaturesFlag::Zeros)
    {
        XInterceptEditBox->Visibility = Windows::UI::Xaml::Visibility::Collapsed;
        XInterceptTextBlock->Visibility = Windows::UI::Xaml::Visibility::Collapsed;
        error.append((m_resourceLoader->GetString(L"XIntercept") + L", ")->Data());
    }
    else if ((ViewModel->TooComplexFeatures & KeyGraphFeaturesFlag::YIntercept) == KeyGraphFeaturesFlag::YIntercept)
    {
        YInterceptEditBox->Visibility = Windows::UI::Xaml::Visibility::Collapsed;
        YInterceptTextBlock->Visibility = Windows::UI::Xaml::Visibility::Collapsed;
        error.append((m_resourceLoader->GetString(L"YIntercept") + L", ")->Data());
    }
    else if ((ViewModel->TooComplexFeatures & KeyGraphFeaturesFlag::Parity) == KeyGraphFeaturesFlag::Parity)
    {
        ParityEditBox->Visibility = Windows::UI::Xaml::Visibility::Collapsed;
        ParityTextBlock->Visibility = Windows::UI::Xaml::Visibility::Collapsed;
        error.append((m_resourceLoader->GetString(L"Parity") + L", ")->Data());
    }
    else if ((ViewModel->TooComplexFeatures & KeyGraphFeaturesFlag::Periodicity) == KeyGraphFeaturesFlag::Periodicity)
    {
        PeriodicityEditBox->Visibility = Windows::UI::Xaml::Visibility::Collapsed;
        PeriodicityTextBlock->Visibility = Windows::UI::Xaml::Visibility::Collapsed;
        error.append((m_resourceLoader->GetString(L"Periodicity") + L", ")->Data());
    }
    else if ((ViewModel->TooComplexFeatures & KeyGraphFeaturesFlag::Minima) == KeyGraphFeaturesFlag::Minima)
    {
        MinimaEditBox->Visibility = Windows::UI::Xaml::Visibility::Collapsed;
        MinimaTextBlock->Visibility = Windows::UI::Xaml::Visibility::Collapsed;
        error.append((m_resourceLoader->GetString(L"Minima") + L", ")->Data());
    }
    else if ((ViewModel->TooComplexFeatures & KeyGraphFeaturesFlag::Maxima) == KeyGraphFeaturesFlag::Maxima)
    {
        MaximaEditBox->Visibility = Windows::UI::Xaml::Visibility::Collapsed;
        MaximaTextBlock->Visibility = Windows::UI::Xaml::Visibility::Collapsed;
        error.append((m_resourceLoader->GetString(L"Maxima") + L", ")->Data());
    }
    else if ((ViewModel->TooComplexFeatures & KeyGraphFeaturesFlag::InflectionPoints) == KeyGraphFeaturesFlag::InflectionPoints)
    {
        InflectionPointsEditBox->Visibility = Windows::UI::Xaml::Visibility::Collapsed;
        InflectionPointsTextBlock->Visibility = Windows::UI::Xaml::Visibility::Collapsed;
        error.append((m_resourceLoader->GetString(L"InflectionPoints") + L", ")->Data());
    }
    else if ((ViewModel->TooComplexFeatures & KeyGraphFeaturesFlag::VerticalAsymptotes) == KeyGraphFeaturesFlag::VerticalAsymptotes)
    {
        VerticalAsymptotesEditBox->Visibility = Windows::UI::Xaml::Visibility::Collapsed;
        VerticalAsymptotesTextBlock->Visibility = Windows::UI::Xaml::Visibility::Collapsed;
        error.append((m_resourceLoader->GetString(L"VerticalAsymptotes") + L", ")->Data());
    }
    else if ((ViewModel->TooComplexFeatures & KeyGraphFeaturesFlag::HorizontalAsymptotes) == KeyGraphFeaturesFlag::HorizontalAsymptotes)
    {
        HorizontalAsymptotesEditBox->Visibility = Windows::UI::Xaml::Visibility::Collapsed;
        HorizontalAsymptotesTextBlock->Visibility = Windows::UI::Xaml::Visibility::Collapsed;
        error.append((m_resourceLoader->GetString(L"HorizontalAsymptotes") + L", ")->Data());
    }
    else if ((ViewModel->TooComplexFeatures & KeyGraphFeaturesFlag::ObliqueAsymptotes) == KeyGraphFeaturesFlag::ObliqueAsymptotes)
    {
        ObliqueAsymptotesEditBox->Visibility = Windows::UI::Xaml::Visibility::Collapsed;
        ObliqueAsymptotesTextBlock->Visibility = Windows::UI::Xaml::Visibility::Collapsed;
        error.append((m_resourceLoader->GetString(L"ObliqueAsymptotes") + L", ")->Data());
    }
    else if ((ViewModel->TooComplexFeatures & KeyGraphFeaturesFlag::MonotoneIntervals) == KeyGraphFeaturesFlag::MonotoneIntervals)
    {
        MonotonicityEditBox->Visibility = Windows::UI::Xaml::Visibility::Collapsed;
        MonotonicityTextBlock->Visibility = Windows::UI::Xaml::Visibility::Collapsed;
        error.append((m_resourceLoader->GetString(L"Monotonicity") + L", ")->Data());
    }

    TooComplexFeaturesError += ref new String(error.substr(0, (error.length() - 2)).c_str());
    TooComplexFeaturesError += L"\n";
    TooComplexFeaturesError += m_resourceLoader->GetString(L"KGFTooComplexFeaturesError");
}

void KeyGraphFeaturesPanel::EquationButtonClicked(Object ^ sender, RoutedEventArgs ^ e)
{
    KeyGraphFeaturesClosed(this, ref new RoutedEventArgs());
}
