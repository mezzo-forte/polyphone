/***************************************************************************
**                                                                        **
**  Polyphone, a soundfont editor                                         **
**  Copyright (C) 2013-2015 Davy Triponney                                **
**                                                                        **
**  This program is free software: you can redistribute it and/or modify  **
**  it under the terms of the GNU General Public License as published by  **
**  the Free Software Foundation, either version 3 of the License, or     **
**  (at your option) any later version.                                   **
**                                                                        **
**  This program is distributed in the hope that it will be useful,       **
**  but WITHOUT ANY WARRANTY; without even the implied warranty of        **
**  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         **
**  GNU General Public License for more details.                          **
**                                                                        **
**  You should have received a copy of the GNU General Public License     **
**  along with this program.  If not, see http://www.gnu.org/licenses/.   **
**                                                                        **
****************************************************************************
**           Author: Davy Triponney                                       **
**  Website/Contact: http://www.polyphone.fr/                             **
**             Date: 01.01.2013                                           **
***************************************************************************/

#include "pageoverviewprst.h"
#include "config.h"

PageOverviewPrst::PageOverviewPrst(QWidget * parent) : PageOverview(PAGE_PRST, elementPrst, parent) {}

QString PageOverviewPrst::getTitle()
{
    return trUtf8("Liste des presets");
}

QStringList PageOverviewPrst::getHorizontalHeader()
{
    QStringList hHeader;
    hHeader << trUtf8("Banque - Preset")
            << trUtf8("Nombre d'instruments")
            << trUtf8("Nombre de paramètres")
            << trUtf8("Nombre de modulateurs")
            << trUtf8("Étendue totale (note)")
            << trUtf8("Étendue totale (vélocité)")
            << trUtf8("Atténuation")
            << trUtf8("Chorus")
            << trUtf8("Réverbération");
    return hHeader;
}

// Called for each preset
QStringList PageOverviewPrst::getInformation(EltID id)
{
    QStringList info;
    info << getBankAndPreset(id)
         << getSampleNumber(id)
         << getParameterNumber(id)
         << getModulatorNumber(id)
         << getKeyRange(id)
         << getVelocityRange(id)
         << getAttenuation(id)
         << getChorus(id)
         << getReverb(id);
    return info;
}

QString PageOverviewPrst::getBankAndPreset(EltID id)
{
    return QString::number(sf2->get(id, champ_wBank).wValue) + " - " + QString::number(sf2->get(id, champ_wPreset).wValue);
}

QString PageOverviewPrst::getSampleNumber(EltID id)
{
    id.typeElement = elementPrstInst;
    return QString::number(sf2->count(id, false));
}

QString PageOverviewPrst::getParameterNumber(EltID id)
{
    int count = 0;

    // Parameters for the global division
    id.typeElement = elementPrstGen;
    count += sf2->count(id);

    // Parameters for the instrument divisions
    id.typeElement = elementPrstInst;
    int nbElt = sf2->count(id);
    for (int i = 0; i < nbElt; i++)
    {
        id.indexElt2 = i;
        if (!sf2->get(id, champ_hidden).bValue)
        {
            EltID idGen = id;
            idGen.typeElement = elementPrstInstGen;
            count += sf2->count(idGen);
        }
    }

    return QString::number(count);
}

QString PageOverviewPrst::getModulatorNumber(EltID id)
{
    int count = 0;

    // Modulators for the global division
    id.typeElement = elementPrstMod;
    count += sf2->count(id);

    // Modulators for the instrument divisions
    id.typeElement = elementPrstInst;
    int nbElt = sf2->count(id);
    for (int i = 0; i < nbElt; i++)
    {
        id.indexElt2 = i;
        if (!sf2->get(id, champ_hidden).bValue)
        {
            EltID idMod = id;
            idMod.typeElement = elementPrstInstMod;
            count += sf2->count(idMod);
        }
    }

    return QString::number(count);
}

QString PageOverviewPrst::getKeyRange(EltID id)
{
    // Global keyrange
    int globalMin = 0;
    int globalMax = 127;
    if (sf2->isSet(id, champ_keyRange))
    {
        rangesType range = sf2->get(id, champ_keyRange).rValue;
        globalMin = range.byLo;
        globalMax = range.byHi;
    }

    // Keyrange per division
    int min = 127;
    int max = 0;
    id.typeElement = elementPrstInst;
    int nbElt = sf2->count(id);
    for (int i = 0; i < nbElt; i++)
    {
        id.indexElt2 = i;
        if (!sf2->get(id, champ_hidden).bValue)
        {
            if (sf2->isSet(id, champ_keyRange))
            {
                rangesType range = sf2->get(id, champ_keyRange).rValue;
                min = qMin(min, (int)range.byLo);
                max = qMax(max, (int)range.byHi);
            }
            else
            {
                min = qMin(min, globalMin);
                max = qMax(max, globalMax);
            }
        }
    }

    if (min > max)
        return "?";
    else if (min == max)
        return Config::getInstance()->getKeyName(min);
    else
        return Config::getInstance()->getKeyName(min) + " - " + Config::getInstance()->getKeyName(max);
}

QString PageOverviewPrst::getVelocityRange(EltID id)
{
    // Global velocity range
    int globalMin = 0;
    int globalMax = 127;
    if (sf2->isSet(id, champ_velocity))
    {
        rangesType range = sf2->get(id, champ_velocity).rValue;
        globalMin = range.byLo;
        globalMax = range.byHi;
    }

    // Velocity range per division
    int min = 127;
    int max = 0;
    id.typeElement = elementPrstInst;
    int nbElt = sf2->count(id);
    for (int i = 0; i < nbElt; i++)
    {
        id.indexElt2 = i;
        if (!sf2->get(id, champ_hidden).bValue)
        {
            if (sf2->isSet(id, champ_velocity))
            {
                rangesType range = sf2->get(id, champ_velocity).rValue;
                min = qMin(min, (int)range.byLo);
                max = qMax(max, (int)range.byHi);
            }
            else
            {
                min = qMin(min, globalMin);
                max = qMax(max, globalMax);
            }
        }
    }

    if (min > max)
        return "?";
    else if (min == max)
        return QString::number(min);
    else
        return QString::number(min) + " - " + QString::number(max);
}

QString PageOverviewPrst::getAttenuation(EltID id)
{
    return getRange(id, champ_initialAttenuation);
}

QString PageOverviewPrst::getChorus(EltID id)
{
    return getRange(id, champ_chorusEffectsSend);
}

QString PageOverviewPrst::getReverb(EltID id)
{
    return getRange(id, champ_reverbEffectsSend);
}