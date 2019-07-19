
#include "pch.h"
#include "GraphingCalculator.xaml.h"
#include "CalcViewModel/Common/KeyboardShortcutManager.h"
#include "Controls/CalculationResult.h"

using namespace CalculatorApp;
using namespace CalculatorApp::Common;
using namespace CalculatorApp::Controls;
using namespace CalculatorApp::ViewModel;
using namespace concurrency;
using namespace GraphControl;
using namespace Platform;
using namespace std::chrono;
using namespace Utils;
using namespace Windows::Foundation::Collections;
using namespace Windows::Storage::Streams;
using namespace Windows::System;
using namespace Windows::UI::Xaml;
using namespace Windows::UI::Xaml::Data;
using namespace Windows::UI::Xaml::Input;
using namespace Windows::UI::Xaml::Media;
using namespace Windows::UI::Xaml::Media::Imaging;

using namespace Windows::Foundation;
using namespace Windows::ApplicationModel::DataTransfer;
using namespace Windows::Storage::Streams;
using namespace Windows::UI::Xaml::Media;

using namespace Windows::UI::Core;

namespace MathSolverEngine
{
    namespace Graph
    {
        namespace Renderer
        {
            struct IBitmap;

        }
    }
}

constexpr auto sc_ViewModelPropertyName = L"ViewModel";

GraphingCalculator::GraphingCalculator()
{
    Equation::RegisterDependencyProperties();
    Grapher::RegisterDependencyProperties();
    InitializeComponent();

    DataTransferManager^ dataTransferManager = DataTransferManager::GetForCurrentView();

    // Register the current control as a share source.
    dataRequestedToken = dataTransferManager->DataRequested += ref new TypedEventHandler<DataTransferManager^, DataRequestedEventArgs^>(this, &GraphingCalculator::OnDataRequested);
}

void GraphingCalculator::GraphingCalculator_DataContextChanged(FrameworkElement^ sender, DataContextChangedEventArgs^ args)
{
    ViewModel = dynamic_cast<GraphingCalculatorViewModel^>(args->NewValue);
}

GraphingCalculatorViewModel^ GraphingCalculator::ViewModel::get()
{
    return m_viewModel;
}

void GraphingCalculator::ViewModel::set(GraphingCalculatorViewModel^ vm)
{
    if (m_viewModel != vm)
    {
        m_viewModel = vm;
        RaisePropertyChanged(StringReference(sc_ViewModelPropertyName));
    }
}


void CalculatorApp::GraphingCalculator::OnShareClick(Platform::Object^ sender, Windows::UI::Xaml::RoutedEventArgs^ e)
{
    // Ask the OS to start a share action.
    if (TheGrapher != nullptr)
    {
        DataTransferManager::ShowShareUI();
    }

}

// When share is invoked (by the user or programatically) the event handler we registered will be called to populate the datapackage with the
// data to be shared.  We will request the current graph image from teh grapher as a stream that will pass to the share request.
void GraphingCalculator::OnDataRequested(DataTransferManager^ sender, DataRequestedEventArgs^ args)
{
    if (TheGrapher != nullptr)
    {
        // Get the current graph 
        auto stream = TheGrapher->GetGraphBitmapStream();

        // If if didn't fail
        if (stream != nullptr)
        {
            // Shortcut to the request data
            auto requestData = args->Request->Data;

            // Todo: where should we get this string from?
            requestData->Properties->Title = L"Look what I graphed";

            // Set the thumbnail image
            requestData->Properties->Thumbnail = stream;

            // And the bitmap
            requestData->SetBitmap(stream);
        }
    }
}

