#ifndef ScoreboardScene_HPP
#define ScoreboardScene_HPP

#include <memory>
#include <vector>
#include <string>

#include "Engine/IScene.hpp"
#include "UI/Component/Label.hpp"
#include "UI/Component/ImageButton.hpp"
#include <allegro5/allegro_audio.h>


struct ScoreEntry {
    std::string name;
    int score;
    std::string datetime; // optional, may be empty
};

class ScoreboardScene final : public Engine::IScene {
private:
    std::shared_ptr<ALLEGRO_SAMPLE_INSTANCE> bgmInstance;
    std::vector<ScoreEntry> entries;
    unsigned int currentPage;
    unsigned int entriesPerPage;

    void LoadScoreboard();

    void SortEntries();

    void ShowPage(unsigned int page);

    void SaveScoreboard();

public:
    ScoreboardScene();

    ~ScoreboardScene() override;

    bool ChangingPage = false;


    void Initialize() override;

    void Terminate() override;

    void BackOnClick(int stage);

    void BGMSlideOnValueChanged(float value);

    void SFXSlideOnValueChanged(float value);

    void PrevPageOnClick(int dummy);

    void NextPageOnClick(int dummy);

    void Draw() const;

    void OnKeyDown(int keyCode) override;
};

#endif  // ScoreboardScene_HPP
