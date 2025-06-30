#include "ClickableArea.h"
#include "TitleBar.h"
#include "euroscope/EuroscopeRadarLoopbackInterface.h"
#include "graphics/GdiGraphicsInterface.h"
#include "graphics/GlobalColours.h"

namespace UKControllerPlugin::Components {
    TitleBar::~TitleBar() = default;

    std::shared_ptr<TitleBar> TitleBar::Create(std::wstring title, Gdiplus::Rect area)
    {
        return std::shared_ptr<TitleBar>(new TitleBar(title, area));
    }

    std::shared_ptr<TitleBar> TitleBar::WithBackgroundBrush(std::shared_ptr<Gdiplus::Brush> brush)
    {
        this->backgroundBrush = brush;
        return shared_from_this();
    }

    std::shared_ptr<TitleBar> TitleBar::WithTextBrush(std::shared_ptr<Gdiplus::Brush> brush)
    {
        this->textBrush = brush;
        return shared_from_this();
    }

    std::shared_ptr<TitleBar> TitleBar::WithBorder(std::shared_ptr<Gdiplus::Pen> pen)
    {
        this->borderPen = pen;
        return shared_from_this();
    }

    std::shared_ptr<TitleBar> TitleBar::WithDrag(int screenObjectId)
    {
        this->clickableArea = ClickableArea::Create(this->area, screenObjectId, "titleBar", true);
        return shared_from_this();
    }

    std::shared_ptr<TitleBar> TitleBar::WithPosition(Gdiplus::Rect area)
    {
        this->area = std::move(area);
        if (this->clickableArea != nullptr) {
            this->clickableArea->WithPosition(this->area);
        }
        return shared_from_this();
    }

    std::shared_ptr<TitleBar> TitleBar::WithTitle(std::wstring title)
    {
        this->title = title;
        return shared_from_this();
    }

    void TitleBar::Draw(
        Windows::GdiGraphicsInterface& graphics, Euroscope::EuroscopeRadarLoopbackInterface& radarScreen) const
    {
        if (this->backgroundBrush) {
            graphics.FillRect(this->area, *this->backgroundBrush);
        }

        if (this->textBrush) {
            graphics.DrawString(this->title, this->area, *this->textBrush);
        }

        if (this->borderPen) {
            graphics.DrawRect(this->area, *this->borderPen);
        }

        if (this->clickableArea != nullptr) {
            this->clickableArea->Apply(graphics, radarScreen);
        }
    }

    TitleBar::TitleBar(std::wstring title, Gdiplus::Rect area) : title(std::move(title)), area(area)
    {
    }

    std::shared_ptr<TitleBar> TitleBar::WithDefaultBackgroundBrush()
    {
        return this->WithBackgroundBrush(std::make_shared<Gdiplus::SolidBrush>(UKControllerPlugin::Graphics::Headers));
    }

    std::shared_ptr<TitleBar> TitleBar::WithDefaultTextBrush()
    {
        return this->WithTextBrush(std::make_shared<Gdiplus::SolidBrush>(UKControllerPlugin::Graphics::DefaultText));
    }

    std::shared_ptr<TitleBar> TitleBar::WithDefaultBorder()
    {
        return this->WithBorder(std::make_shared<Gdiplus::Pen>(UKControllerPlugin::Graphics::Border));
    }
} // namespace UKControllerPlugin::Components
